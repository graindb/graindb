#include "duckdb/storage/data_table.hpp"

#include "duckdb/catalog/catalog_entry/table_catalog_entry.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/common/helper.hpp"
#include "duckdb/common/vector_operations/vector_operations.hpp"
#include "duckdb/execution/expression_executor.hpp"
#include "duckdb/main/client_context.hpp"
#include "duckdb/planner/constraints/list.hpp"
#include "duckdb/storage/storage_manager.hpp"
#include "duckdb/storage/string_segment.hpp"
#include "duckdb/storage/table/transient_segment.hpp"
#include "duckdb/transaction/transaction.hpp"
#include "duckdb/transaction/transaction_manager.hpp"

#include <iostream>

using namespace duckdb;
using namespace std;
using namespace chrono;

DataTable::DataTable(StorageManager &storage, string schema, string table, vector<TypeId> types_,
                     unique_ptr<vector<unique_ptr<PersistentSegment>>[]> data)
    : info(make_shared<DataTableInfo>(schema, table)), types(types_), storage(storage),
      persistent_manager(make_shared<VersionManager>(*info)), transient_manager(make_shared<VersionManager>(*info)),
      is_root(true) {
	// set up the segment trees for the column segments
	for (idx_t i = 0; i < types.size(); i++) {
		auto column_data = make_shared<ColumnData>(*storage.buffer_manager, *info);
		column_data->type = types[i];
		column_data->column_idx = i;
		columns.push_back(move(column_data));
	}

	// initialize the table with the existing data from disk, if any
	if (data && data[0].size() > 0) {
		// first append all the segments to the set of column segments
		for (idx_t i = 0; i < types.size(); i++) {
			columns[i]->Initialize(data[i]);
			if (columns[i]->persistent_rows != columns[0]->persistent_rows) {
				throw Exception("Column length mismatch in table load!");
			}
		}
		persistent_manager->max_row = columns[0]->persistent_rows;
		transient_manager->base_row = persistent_manager->max_row;
	}
}

DataTable::DataTable(ClientContext &context, DataTable &parent, ColumnDefinition &new_column, Expression *default_value)
    : info(parent.info), types(parent.types), storage(parent.storage), persistent_manager(parent.persistent_manager),
      transient_manager(parent.transient_manager), columns(parent.columns), is_root(true) {
	// prevent any new tuples from being added to the parent
	lock_guard<mutex> parent_lock(parent.append_lock);
	// add the new column to this DataTable
	auto new_column_type = GetInternalType(new_column.type);
	idx_t new_column_idx = columns.size();

	types.push_back(new_column_type);
	auto column_data = make_shared<ColumnData>(*storage.buffer_manager, *info);
	column_data->type = new_column_type;
	column_data->column_idx = new_column_idx;
	columns.push_back(move(column_data));

	// fill the column with its DEFAULT value, or NULL if none is specified
	idx_t rows_to_write = persistent_manager->max_row + transient_manager->max_row;
	if (rows_to_write > 0) {
		ExpressionExecutor executor;
		DataChunk dummy_chunk;
		Vector result(new_column_type);
		if (!default_value) {
			FlatVector::Nullmask(result).set();
		} else {
			executor.AddExpression(*default_value);
		}

		ColumnAppendState state;
		columns[new_column_idx]->InitializeAppend(state);
		for (idx_t i = 0; i < rows_to_write; i += STANDARD_VECTOR_SIZE) {
			idx_t rows_in_this_vector = std::min(rows_to_write - i, (idx_t)STANDARD_VECTOR_SIZE);
			if (default_value) {
				dummy_chunk.SetCardinality(rows_in_this_vector);
				executor.ExecuteExpression(dummy_chunk, result);
			}
			columns[new_column_idx]->Append(state, result, rows_in_this_vector);
		}
	}
	// also add this column to client local storage
	Transaction::GetTransaction(context).storage.AddColumn(&parent, this, new_column, default_value);

	// this table replaces the previous table, hence the parent is no longer the root DataTable
	parent.is_root = false;
}

DataTable::DataTable(ClientContext &context, DataTable &parent, idx_t removed_column)
    : info(parent.info), types(parent.types), storage(parent.storage), persistent_manager(parent.persistent_manager),
      transient_manager(parent.transient_manager), columns(parent.columns), is_root(true) {
	// prevent any new tuples from being added to the parent
	lock_guard<mutex> parent_lock(parent.append_lock);
	// first check if there are any indexes that exist that point to the removed column
	for (auto &index : info->indexes) {
		for (auto &column_id : index->column_ids) {
			if (column_id == removed_column) {
				throw CatalogException("Cannot drop this column: an index depends on it!");
			} else if (column_id > removed_column) {
				throw CatalogException("Cannot drop this column: an index depends on a column after it!");
			}
		}
	}
	// erase the column from this DataTable
	assert(removed_column < types.size());
	types.erase(types.begin() + removed_column);
	columns.erase(columns.begin() + removed_column);

	// this table replaces the previous table, hence the parent is no longer the root DataTable
	parent.is_root = false;
}

DataTable::DataTable(ClientContext &context, DataTable &parent, idx_t changed_idx, SQLType target_type,
                     vector<column_t> bound_columns, Expression &cast_expr)
    : info(parent.info), types(parent.types), storage(parent.storage), persistent_manager(parent.persistent_manager),
      transient_manager(parent.transient_manager), columns(parent.columns), is_root(true) {

	// prevent any new tuples from being added to the parent
	CreateIndexScanState scan_state;
	parent.InitializeCreateIndexScan(scan_state, bound_columns);

	// first check if there are any indexes that exist that point to the changed column
	for (auto &index : info->indexes) {
		for (auto &column_id : index->column_ids) {
			if (column_id == changed_idx) {
				throw CatalogException("Cannot change the type of this column: an index depends on it!");
			}
		}
	}
	// change the type in this DataTable
	auto new_type = GetInternalType(target_type);
	types[changed_idx] = new_type;

	// construct a new column data for this type
	auto column_data = make_shared<ColumnData>(*storage.buffer_manager, *info);
	column_data->type = new_type;
	column_data->column_idx = changed_idx;

	ColumnAppendState append_state;
	column_data->InitializeAppend(append_state);

	// scan the original table, and fill the new column with the transformed value
	auto &transaction = Transaction::GetTransaction(context);

	vector<TypeId> types;
	for (idx_t i = 0; i < bound_columns.size(); i++) {
		if (bound_columns[i] == COLUMN_IDENTIFIER_ROW_ID) {
			types.push_back(ROW_TYPE);
		} else {
			types.push_back(parent.types[bound_columns[i]]);
		}
	}

	DataChunk scan_chunk;
	scan_chunk.Initialize(types);

	ExpressionExecutor executor;
	executor.AddExpression(cast_expr);

	Vector append_vector(new_type);
	while (true) {
		// scan the table
		scan_chunk.Reset();
		parent.CreateIndexScan(scan_state, scan_chunk);
		if (scan_chunk.size() == 0) {
			break;
		}
		// execute the expression
		executor.ExecuteExpression(scan_chunk, append_vector);
		column_data->Append(append_state, append_vector, scan_chunk.size());
	}
	// also add this column to client local storage
	transaction.storage.ChangeType(&parent, this, changed_idx, target_type, bound_columns, cast_expr);

	columns[changed_idx] = move(column_data);

	// this table replaces the previous table, hence the parent is no longer the root DataTable
	parent.is_root = false;
}

//===--------------------------------------------------------------------===//
// Scan
//===--------------------------------------------------------------------===//
void DataTable::InitializeScan(TableScanState &state, vector<column_t> column_ids,
                               unordered_map<idx_t, vector<TableFilter>> *table_filters) {
	// initialize a column scan state for each column
	state.column_scans = unique_ptr<ColumnScanState[]>(new ColumnScanState[column_ids.size()]);
	for (idx_t i = 0; i < column_ids.size(); i++) {
		auto column = column_ids[i];
		if (column != COLUMN_IDENTIFIER_ROW_ID) {
			columns[column]->InitializeScan(state.column_scans[i]);
		}
	}
	state.column_ids = move(column_ids);
	// initialize the chunk scan state
	state.offset = 0;
	state.current_persistent_row = 0;
	state.max_persistent_row = persistent_manager->max_row;
	state.current_transient_row = 0;
	state.max_transient_row = transient_manager->max_row;
	if (table_filters && table_filters->size() > 0) {
		state.adaptive_filter = make_unique<AdaptiveFilter>(*table_filters);
	}
}

void DataTable::InitializeScan(TableScanState &state, vector<column_t> column_ids,
                               const shared_ptr<rows_vector> &rowids_, idx_t rows_count_,
                               unordered_map<idx_t, vector<TableFilter>> *table_filters) {
	InitializeScan(state, move(column_ids), table_filters);
	state.rowids = rowids_;
	state.rows_offset = 0;
	state.rows_count = rows_count_;
}

void DataTable::InitializeScan(TableScanState &state, vector<column_t> column_ids,
                               const shared_ptr<bitmask_vector> &zones_, const shared_ptr<bitmask_vector> &zones_sel,
                               unordered_map<idx_t, vector<TableFilter>> *table_filters) {
	InitializeScan(state, move(column_ids), table_filters);
	state.zones = zones_;
	state.zones_sel = zones_sel;
}

void DataTable::InitializeScan(Transaction &transaction, TableScanState &state, vector<column_t> column_ids,
                               unordered_map<idx_t, vector<TableFilter>> *table_filters) {
	InitializeScan(state, move(column_ids), table_filters);
	transaction.storage.InitializeScan(this, state.local_state);
}

void DataTable::Scan(Transaction &transaction, DataChunk &result, TableScanState &state,
                     unordered_map<idx_t, vector<TableFilter>> &table_filters) {
	// scan the persistent segments
	while (ScanBaseTable(transaction, result, state, state.current_persistent_row, state.max_persistent_row, 0,
	                     *persistent_manager, table_filters)) {
		if (result.size() > 0) {
			return;
		}
	}
	// scan the transient segments
	while (ScanBaseTable(transaction, result, state, state.current_transient_row, state.max_transient_row,
	                     persistent_manager->max_row, *transient_manager, table_filters)) {
		if (result.size() > 0) {
			return;
		}
	}

	// scan the transaction-local segments
	transaction.storage.Scan(state.local_state, state.column_ids, result, &table_filters);
}

template <class T> bool checkZonemap(SegmentStatistics &stats, TableFilter &table_filter, T constant) {
	T *min = (T *)stats.minimum.get();
	T *max = (T *)stats.maximum.get();
	switch (table_filter.comparison_type) {
	case ExpressionType::COMPARE_EQUAL:
		return constant >= *min && constant <= *max;
	case ExpressionType::COMPARE_GREATERTHANOREQUALTO:
		return constant <= *max;
	case ExpressionType::COMPARE_GREATERTHAN:
		return constant < *max;
	case ExpressionType::COMPARE_LESSTHANOREQUALTO:
		return constant >= *min;
	case ExpressionType::COMPARE_LESSTHAN:
		return constant > *min;
	default:
		throw NotImplementedException("Operation not implemented");
	}
}

template <class T> bool checkZonemap(TableScanState &state, TableFilter &table_filter, T constant) {
	return checkZonemap<T>(state.column_scans[table_filter.column_index].current->stats, table_filter, constant);
}

bool checkZonemapString(SegmentStatistics &stats, TableFilter &table_filter, const char *constant) {
	char *min = (char *)stats.minimum.get();
	char *max = (char *)stats.maximum.get();
	int min_comp = strcmp(min, constant);
	int max_comp = strcmp(max, constant);
	switch (table_filter.comparison_type) {
	case ExpressionType::COMPARE_EQUAL:
		return min_comp <= 0 && max_comp >= 0;
	case ExpressionType::COMPARE_GREATERTHANOREQUALTO:
	case ExpressionType::COMPARE_GREATERTHAN:
		return max_comp >= 0;
	case ExpressionType::COMPARE_LESSTHAN:
	case ExpressionType::COMPARE_LESSTHANOREQUALTO:
		return min_comp <= 0;
	default:
		throw NotImplementedException("Operation not implemented");
	}
}

bool checkZonemapString(TableScanState &state, TableFilter &table_filter, const char *constant) {
	return checkZonemapString(state.column_scans[table_filter.column_index].current->stats, table_filter, constant);
}

bool DataTable::CheckZonemap(TableScanState &state, unordered_map<idx_t, vector<TableFilter>> &table_filters,
                             idx_t &current_row, SelectionVector &valid_sel, idx_t &sel_count) {
	if (state.zones) {
		auto &current_zones = *state.zones;
		auto zone_id = current_row / STANDARD_VECTOR_SIZE;
		auto zone_count = state.zones_sel->operator[](zone_id) * STANDARD_VECTOR_SIZE;
		sel_count = 0;
		for (uint16_t i = 0; i < zone_count; i++) {
			valid_sel.set_index(sel_count, i);
			sel_count += current_zones[i + current_row];
		}
	}

	for (auto &table_filter : table_filters) {
		for (auto &predicate_constant : table_filter.second) {
			bool readSegment = true;
			if (!state.column_scans[predicate_constant.column_index].segment_checked) {
				state.column_scans[predicate_constant.column_index].segment_checked = true;
				auto current_segment = state.column_scans[predicate_constant.column_index].current;
				if (!current_segment) {
					return true;
				}
				switch (current_segment->type) {
				case TypeId::INT8: {
					int8_t constant = predicate_constant.constant.value_.tinyint;
					readSegment = checkZonemap<int8_t>(state, predicate_constant, constant);
					break;
				}
				case TypeId::INT16: {
					int16_t constant = predicate_constant.constant.value_.smallint;
					readSegment = checkZonemap<int16_t>(state, predicate_constant, constant);
					break;
				}
				case TypeId::INT32: {
					int32_t constant = predicate_constant.constant.value_.integer;
					readSegment = checkZonemap<int32_t>(state, predicate_constant, constant);
					break;
				}
				case TypeId::INT64: {
					int64_t constant = predicate_constant.constant.value_.bigint;
					readSegment = checkZonemap<int64_t>(state, predicate_constant, constant);
					break;
				}
				case TypeId::FLOAT: {
					float constant = predicate_constant.constant.value_.float_;
					readSegment = checkZonemap<float>(state, predicate_constant, constant);
					break;
				}
				case TypeId::DOUBLE: {
					double constant = predicate_constant.constant.value_.double_;
					readSegment = checkZonemap<double>(state, predicate_constant, constant);
					break;
				}
				case TypeId::VARCHAR: {
					//! we can only compare the first 7 bytes
					size_t value_size = predicate_constant.constant.str_value.size() > 7
					                        ? 7
					                        : predicate_constant.constant.str_value.size();
					string constant;
					for (size_t i = 0; i < value_size; i++) {
						constant += predicate_constant.constant.str_value[i];
					}
					readSegment = checkZonemapString(state, predicate_constant, constant.c_str());
					break;
				}
				default:
					throw NotImplementedException("Unimplemented type for uncompressed segment");
				}
			}
			if (!readSegment) {
				//! We can skip this partition
				idx_t vectorsToSkip =
				    ceil((double)(state.column_scans[predicate_constant.column_index].current->count +
				                  state.column_scans[predicate_constant.column_index].current->start - current_row) /
				         STANDARD_VECTOR_SIZE);
				for (idx_t i = 0; i < vectorsToSkip; ++i) {
					state.NextVector();
					current_row += STANDARD_VECTOR_SIZE;
				}
				return false;
			}
		}
	}

	return true;
}

template <class T>
void inline DataTable::LookupRows(shared_ptr<ColumnData> column, const shared_ptr<vector<row_t>> &rowids,
                                  Vector &result, idx_t offset, idx_t count, idx_t type_size) {
	auto result_data = FlatVector::GetData(result);
	idx_t s_size = column->data.nodes[0].node->count;
	for (idx_t i = 0; i < count; i++) {
		row_t row_id = rowids->operator[](i + offset);
		idx_t s_index = row_id / s_size;
		idx_t s_offset = row_id % s_size;
		idx_t vector_index = s_offset / STANDARD_VECTOR_SIZE;
		idx_t id_in_vector = s_offset - vector_index * STANDARD_VECTOR_SIZE;
		auto transient_segment = (TransientSegment *)column->data.nodes[s_index].node;
		auto s_data = transient_segment->data->block_buffer +
		              (vector_index * (sizeof(nullmask_t) + type_size * STANDARD_VECTOR_SIZE) + sizeof(nullmask_t));
		memcpy(result_data + (i * type_size), s_data + (id_in_vector * type_size), type_size);
	}
}

void DataTable::PerformLookups(Transaction &transaction, TableScanState &state, DataChunk &result,
                               const shared_ptr<vector<row_t>> &rowids, idx_t offset, idx_t size) {
	result.SetCardinality(size);
	for (idx_t col_idx = 0; col_idx < state.column_ids.size(); col_idx++) {
		auto cid = state.column_ids[col_idx];
		if (cid == COLUMN_IDENTIFIER_ROW_ID) {
			assert(result.data[col_idx].type == TypeId::INT64);
			result.data[col_idx].vector_type = VectorType::FLAT_VECTOR;
			auto data = FlatVector::GetData(result.data[col_idx]);
			memcpy(data, (data_ptr_t)rowids->data() + (offset * sizeof(int64_t)), size * sizeof(int64_t));
			continue;
		}
		auto col = columns[cid];
		switch (col->type) {
		case TypeId::INT8: {
			LookupRows<int8_t>(col, rowids, result.data[col_idx], offset, size);
			break;
		}
		case TypeId::UINT8: {
			LookupRows<uint8_t>(col, rowids, result.data[col_idx], offset, size);
			break;
		}
		case TypeId::INT16: {
			LookupRows<int16_t>(col, rowids, result.data[col_idx], offset, size);
			break;
		}
		case TypeId::HASH:
		case TypeId::UINT16: {
			LookupRows<uint16_t>(col, rowids, result.data[col_idx], offset, size);
			break;
		}
		case TypeId::INT32: {
			LookupRows<int32_t>(col, rowids, result.data[col_idx], offset, size);
			break;
		}
		case TypeId::UINT32: {
			LookupRows<uint32_t>(col, rowids, result.data[col_idx], offset, size);
			break;
		}
		case TypeId::TIMESTAMP:
		case TypeId::INT64: {
			LookupRows<int64_t>(col, rowids, result.data[col_idx], offset, size);
			break;
		}
		case TypeId::UINT64: {
			LookupRows<uint64_t>(col, rowids, result.data[col_idx], offset, size);
			break;
		}
		case TypeId::FLOAT: {
			LookupRows<float_t>(col, rowids, result.data[col_idx], offset, size);
			break;
		}
		case TypeId::DOUBLE: {
			LookupRows<double_t>(col, rowids, result.data[col_idx], offset, size);
			break;
		}
		case TypeId::POINTER: {
			LookupRows<uintptr_t>(col, rowids, result.data[col_idx], offset, size);
			break;
		}
		default: {
			for (idx_t i = 0; i < size; i++) {
				col->FetchRow(state.fetch_state, transaction, rowids->operator[](i + offset), result.data[col_idx], i);
			}
			break;
		}
		}
	}
}

bool DataTable::ScanBaseTable(Transaction &transaction, DataChunk &result, TableScanState &state, idx_t &current_row,
                              idx_t max_row, idx_t base_row, VersionManager &manager,
                              unordered_map<idx_t, vector<TableFilter>> &table_filters) {
	if (current_row >= max_row) {
		// exceeded the amount of rows to scan
		return false;
	}
	// perform rowid lookups
	if (state.rows_count >= 0) {
		auto scan_count = std::min((idx_t)STANDARD_VECTOR_SIZE, state.rows_count - state.rows_offset);
		PerformLookups(transaction, state, result, state.rowids, state.rows_offset, scan_count);
		state.rows_offset += scan_count;
		current_row += STANDARD_VECTOR_SIZE;
		return true;
	}
	idx_t max_count = std::min((idx_t)STANDARD_VECTOR_SIZE, max_row - current_row);
	idx_t vector_offset = current_row / STANDARD_VECTOR_SIZE;
	// second, scan the version chunk manager to figure out which tuples to load for this transaction
	SelectionVector valid_sel;
	valid_sel.InitializeCopy((sel_t *)FlatVector::incremental_vector);
	idx_t sel_count = max_count;
	//! first check the zonemap if we have to scan this partition
	if (!CheckZonemap(state, table_filters, current_row, valid_sel, sel_count)) {
		return true;
	}
	idx_t count = manager.GetSelVector(transaction, vector_offset, valid_sel, sel_count);
	if (count == 0) {
		// nothing to scan for this vector, skip the entire vector
		state.NextVector();
		current_row += STANDARD_VECTOR_SIZE;
		return true;
	}
	idx_t approved_tuple_count = count;
	if (table_filters.empty()) {
		//! If we don't have any deleted tuples or filters we can just run a regular scan
		for (idx_t i = 0; i < state.column_ids.size(); i++) {
			auto column = state.column_ids[i];
			if (column == COLUMN_IDENTIFIER_ROW_ID) {
				// scan row id
				assert(result.data[i].type == ROW_TYPE);
				result.data[i].Sequence(base_row + current_row, 1);
			} else {
				columns[column]->Scan(transaction, state.column_scans[i], result.data[i]);
			}
		}
		if (count < max_count) {
			result.Slice(valid_sel, count);
		}
	} else {
		SelectionVector sel;

		if (count != max_count) {
			sel.Initialize(valid_sel);
		} else {
			sel.Initialize(FlatVector::IncrementalSelectionVector);
		}
		//! First, we scan the columns with filters, fetch their data and generate a selection vector.
		//! get runtime statistics
		auto start_time = high_resolution_clock::now();
		for (idx_t i = 0; i < table_filters.size(); i++) {
			auto tf_idx = state.adaptive_filter->permutation[i];
			columns[tf_idx]->Select(transaction, state.column_scans[tf_idx], result.data[tf_idx], sel,
			                        approved_tuple_count, table_filters[tf_idx]);
		}
		for (auto &table_filter : table_filters) {
			result.data[table_filter.first].Slice(sel, approved_tuple_count);
		}
		//! Now we use the selection vector to fetch data for the other columns.
		for (idx_t i = 0; i < state.column_ids.size(); i++) {
			if (table_filters.find(i) == table_filters.end()) {
				auto column = state.column_ids[i];
				if (column == COLUMN_IDENTIFIER_ROW_ID) {
					assert(result.data[i].type == TypeId::INT64);
					result.data[i].vector_type = VectorType::FLAT_VECTOR;
					auto result_data = (int64_t *)FlatVector::GetData(result.data[i]);
					for (size_t sel_idx = 0; sel_idx < approved_tuple_count; sel_idx++) {
						result_data[sel_idx] = base_row + current_row + sel.get_index(sel_idx);
					}
				} else {
					columns[column]->FilterScan(transaction, state.column_scans[i], result.data[i], sel,
					                            approved_tuple_count);
				}
			}
		}
		auto end_time = high_resolution_clock::now();
		if (state.adaptive_filter && table_filters.size() > 1) {
			state.adaptive_filter->AdaptRuntimeStatistics(
			    duration_cast<duration<double>>(end_time - start_time).count());
		}
	}

	result.SetCardinality(approved_tuple_count);
	current_row += STANDARD_VECTOR_SIZE;
	return true;
}

//===--------------------------------------------------------------------===//
// Index Scan
//===--------------------------------------------------------------------===//
void DataTable::InitializeIndexScan(Transaction &transaction, TableIndexScanState &state, Index &index,
                                    vector<column_t> column_ids) {
	state.index = &index;
	state.column_ids = move(column_ids);
	transaction.storage.InitializeScan(this, state.local_state);
}

void DataTable::InitializeIndexScan(Transaction &transaction, TableIndexScanState &state, Index &index, Value value,
                                    ExpressionType expr_type, vector<column_t> column_ids) {
	InitializeIndexScan(transaction, state, index, move(column_ids));
	state.index_state = index.InitializeScanSinglePredicate(transaction, state.column_ids, value, expr_type);
}

void DataTable::InitializeIndexScan(Transaction &transaction, TableIndexScanState &state, Index &index, Value low_value,
                                    ExpressionType low_type, Value high_value, ExpressionType high_type,
                                    vector<column_t> column_ids) {
	InitializeIndexScan(transaction, state, index, move(column_ids));
	state.index_state =
	    index.InitializeScanTwoPredicates(transaction, state.column_ids, low_value, low_type, high_value, high_type);
}

void DataTable::IndexScan(Transaction &transaction, DataChunk &result, TableIndexScanState &state) {
	// clear any previously pinned blocks
	state.fetch_state.handles.clear();
	// scan the index
	state.index->Scan(transaction, *this, state, result);
	if (result.size() > 0) {
		return;
	}
	// scan the local structure
	transaction.storage.Scan(state.local_state, state.column_ids, result);
}

//===--------------------------------------------------------------------===//
// Fetch
//===--------------------------------------------------------------------===//
void DataTable::Fetch(Transaction &transaction, DataChunk &result, vector<column_t> &column_ids,
                      Vector &row_identifiers, idx_t fetch_count, TableIndexScanState &state) {
	// first figure out which row identifiers we should use for this transaction by looking at the VersionManagers
	row_t rows[STANDARD_VECTOR_SIZE];
	idx_t count = FetchRows(transaction, row_identifiers, fetch_count, rows);

	if (count == 0) {
		// no rows to use
		return;
	}
	// for each of the remaining rows, now fetch the data
	result.SetCardinality(count);
	for (idx_t col_idx = 0; col_idx < column_ids.size(); col_idx++) {
		auto column = column_ids[col_idx];
		if (column == COLUMN_IDENTIFIER_ROW_ID) {
			// row id column: fill in the row ids
			assert(result.data[col_idx].type == TypeId::INT64);
			result.data[col_idx].vector_type = VectorType::FLAT_VECTOR;
			auto data = FlatVector::GetData<row_t>(result.data[col_idx]);
			for (idx_t i = 0; i < count; i++) {
				data[i] = rows[i];
			}
		} else {
			// regular column: fetch data from the base column
			for (idx_t i = 0; i < count; i++) {
				auto row_id = rows[i];
				columns[column]->FetchRow(state.fetch_state, transaction, row_id, result.data[col_idx], i);
			}
		}
	}
}

idx_t DataTable::FetchRows(Transaction &transaction, Vector &row_identifiers, idx_t fetch_count, row_t result_rows[]) {
	assert(row_identifiers.type == ROW_TYPE);

	// obtain a read lock on the version managers
	auto l1 = persistent_manager->lock.GetSharedLock();
	auto l2 = transient_manager->lock.GetSharedLock();

	// now iterate over the row ids and figure out which rows to use
	idx_t count = 0;

	auto row_ids = FlatVector::GetData<row_t>(row_identifiers);
	for (idx_t i = 0; i < fetch_count; i++) {
		auto row_id = row_ids[i];
		bool use_row;
		if ((idx_t)row_id < persistent_manager->max_row) {
			// persistent row: use persistent manager
			use_row = persistent_manager->Fetch(transaction, row_id);
		} else {
			// transient row: use transient manager
			use_row = transient_manager->Fetch(transaction, row_id);
		}
		if (use_row) {
			// row is not deleted; use the row
			result_rows[count++] = row_id;
		}
	}
	return count;
}

//===--------------------------------------------------------------------===//
// Append
//===--------------------------------------------------------------------===//
static void VerifyNotNullConstraint(TableCatalogEntry &table, Vector &vector, idx_t count, string &col_name) {
	if (VectorOperations::HasNull(vector, count)) {
		throw ConstraintException("NOT NULL constraint failed: %s.%s", table.name.c_str(), col_name.c_str());
	}
}

static void VerifyCheckConstraint(TableCatalogEntry &table, Expression &expr, DataChunk &chunk) {
	ExpressionExecutor executor(expr);
	Vector result(TypeId::INT32);
	try {
		executor.ExecuteExpression(chunk, result);
	} catch (Exception &ex) {
		throw ConstraintException("CHECK constraint failed: %s (Error: %s)", table.name.c_str(), ex.what());
	} catch (...) {
		throw ConstraintException("CHECK constraint failed: %s (Unknown Error)", table.name.c_str());
	}
	VectorData vdata;
	result.Orrify(chunk.size(), vdata);

	auto dataptr = (int32_t *)vdata.data;
	for (idx_t i = 0; i < chunk.size(); i++) {
		auto idx = vdata.sel->get_index(i);
		if (!(*vdata.nullmask)[idx] && dataptr[idx] == 0) {
			throw ConstraintException("CHECK constraint failed: %s", table.name.c_str());
		}
	}
}

void DataTable::VerifyAppendConstraints(TableCatalogEntry &table, DataChunk &chunk) {
	for (auto &constraint : table.bound_constraints) {
		switch (constraint->type) {
		case ConstraintType::NOT_NULL: {
			auto &not_null = *reinterpret_cast<BoundNotNullConstraint *>(constraint.get());
			VerifyNotNullConstraint(table, chunk.data[not_null.index], chunk.size(),
			                        table.columns[not_null.index].name);
			break;
		}
		case ConstraintType::CHECK: {
			auto &check = *reinterpret_cast<BoundCheckConstraint *>(constraint.get());
			VerifyCheckConstraint(table, *check.expression, chunk);
			break;
		}
		case ConstraintType::UNIQUE: {
			//! check whether or not the chunk can be inserted into the indexes
			for (auto &index : info->indexes) {
				index->VerifyAppend(chunk);
			}
			break;
		}
		case ConstraintType::FOREIGN_KEY:
		default:
			throw NotImplementedException("Constraint type not implemented!");
		}
	}
}

void DataTable::Append(TableCatalogEntry &table, ClientContext &context, DataChunk &chunk) {
	if (chunk.size() == 0) {
		return;
	}
	if (chunk.column_count() != table.columns.size()) {
		throw CatalogException("Mismatch in column count for append");
	}
	if (!is_root) {
		throw TransactionException("Transaction conflict: adding entries to a table that has been altered!");
	}

	chunk.Verify();

	// verify any constraints on the new chunk
	VerifyAppendConstraints(table, chunk);

	// append to the transaction local data
	auto &transaction = Transaction::GetTransaction(context);
	transaction.storage.Append(this, chunk);
}

void DataTable::InitializeAppend(TableAppendState &state) {
	// obtain the append lock for this table
	state.append_lock = unique_lock<mutex>(append_lock);
	if (!is_root) {
		throw TransactionException("Transaction conflict: adding entries to a table that has been altered!");
	}
	// obtain locks on all indexes for the table
	state.index_locks = unique_ptr<IndexLock[]>(new IndexLock[info->indexes.size()]);
	for (idx_t i = 0; i < info->indexes.size(); i++) {
		info->indexes[i]->InitializeLock(state.index_locks[i]);
	}
	// for each column, initialize the append state
	state.states = unique_ptr<ColumnAppendState[]>(new ColumnAppendState[types.size()]);
	for (idx_t i = 0; i < types.size(); i++) {
		columns[i]->InitializeAppend(state.states[i]);
	}
	state.row_start = transient_manager->max_row;
	state.current_row = state.row_start;
}

void DataTable::Append(Transaction &transaction, transaction_t commit_id, DataChunk &chunk, TableAppendState &state) {
	assert(is_root);
	assert(chunk.column_count() == types.size());
	chunk.Verify();

	// set up the inserted info in the version manager
	transient_manager->Append(transaction, state.current_row, chunk.size(), commit_id);

	// append the physical data to each of the entries
	for (idx_t i = 0; i < types.size(); i++) {
		columns[i]->Append(state.states[i], chunk.data[i], chunk.size());
	}
	info->cardinality += chunk.size();
	state.current_row += chunk.size();
}

void DataTable::RevertAppend(TableAppendState &state) {
	if (state.row_start == state.current_row) {
		// nothing to revert!
		return;
	}
	assert(is_root);
	// revert changes in the base columns
	for (idx_t i = 0; i < types.size(); i++) {
		columns[i]->RevertAppend(state.row_start);
	}
	// adjust the cardinality
	info->cardinality -= state.current_row - state.row_start;
	transient_manager->max_row = state.row_start;
	// revert changes in the transient manager
	transient_manager->RevertAppend(state.row_start, state.current_row);
}

//===--------------------------------------------------------------------===//
// Indexes
//===--------------------------------------------------------------------===//
bool DataTable::AppendToIndexes(TableAppendState &state, DataChunk &chunk, row_t row_start) {
	assert(is_root);
	if (info->indexes.size() == 0) {
		return true;
	}
	// first generate the vector of row identifiers
	Vector row_identifiers(ROW_TYPE);
	VectorOperations::GenerateSequence(row_identifiers, chunk.size(), row_start, 1);

	idx_t failed_index = INVALID_INDEX;
	// now append the entries to the indices
	for (idx_t i = 0; i < info->indexes.size(); i++) {
		if (!info->indexes[i]->Append(state.index_locks[i], chunk, row_identifiers)) {
			failed_index = i;
			break;
		}
	}
	if (failed_index != INVALID_INDEX) {
		// constraint violation!
		// remove any appended entries from previous indexes (if any)
		for (idx_t i = 0; i < failed_index; i++) {
			info->indexes[i]->Delete(state.index_locks[i], chunk, row_identifiers);
		}
		return false;
	}
	return true;
}

void DataTable::RemoveFromIndexes(TableAppendState &state, DataChunk &chunk, row_t row_start) {
	assert(is_root);
	if (info->indexes.size() == 0) {
		return;
	}
	// first generate the vector of row identifiers
	Vector row_identifiers(ROW_TYPE);
	VectorOperations::GenerateSequence(row_identifiers, chunk.size(), row_start, 1);

	// now remove the entries from the indices
	RemoveFromIndexes(state, chunk, row_identifiers);
}

void DataTable::RemoveFromIndexes(TableAppendState &state, DataChunk &chunk, Vector &row_identifiers) {
	assert(is_root);
	for (idx_t i = 0; i < info->indexes.size(); i++) {
		info->indexes[i]->Delete(state.index_locks[i], chunk, row_identifiers);
	}
}

void DataTable::RemoveFromIndexes(Vector &row_identifiers, idx_t count) {
	assert(is_root);
	auto row_ids = FlatVector::GetData<row_t>(row_identifiers);
	// create a selection vector from the row_ids
	SelectionVector sel(STANDARD_VECTOR_SIZE);
	for (idx_t i = 0; i < count; i++) {
		sel.set_index(i, row_ids[i] % STANDARD_VECTOR_SIZE);
	}

	// fetch the data for these row identifiers
	DataChunk result;
	result.Initialize(types);
	// FIXME: we do not need to fetch all columns, only the columns required by the indices!
	auto states = unique_ptr<ColumnScanState[]>(new ColumnScanState[types.size()]);
	for (idx_t i = 0; i < types.size(); i++) {
		columns[i]->Fetch(states[i], row_ids[0], result.data[i]);
	}
	result.Slice(sel, count);
	for (idx_t i = 0; i < info->indexes.size(); i++) {
		info->indexes[i]->Delete(result, row_identifiers);
	}
}

//===--------------------------------------------------------------------===//
// Delete
//===--------------------------------------------------------------------===//
void DataTable::Delete(TableCatalogEntry &table, ClientContext &context, Vector &row_identifiers, idx_t count) {
	assert(row_identifiers.type == ROW_TYPE);
	if (count == 0) {
		return;
	}

	auto &transaction = Transaction::GetTransaction(context);

	row_identifiers.Normalify(count);
	auto ids = FlatVector::GetData<row_t>(row_identifiers);
	auto first_id = ids[0];

	if (first_id >= MAX_ROW_ID) {
		// deletion is in transaction-local storage: push delete into local chunk collection
		transaction.storage.Delete(this, row_identifiers, count);
	} else if ((idx_t)first_id < persistent_manager->max_row) {
		// deletion is in persistent storage: delete in the persistent version manager
		persistent_manager->Delete(transaction, this, row_identifiers, count);
	} else {
		// deletion is in transient storage: delete in the persistent version manager
		transient_manager->Delete(transaction, this, row_identifiers, count);
	}
}

//===--------------------------------------------------------------------===//
// Update
//===--------------------------------------------------------------------===//
static void CreateMockChunk(vector<TypeId> &types, vector<column_t> &column_ids, DataChunk &chunk,
                            DataChunk &mock_chunk) {
	// construct a mock DataChunk
	mock_chunk.InitializeEmpty(types);
	for (column_t i = 0; i < column_ids.size(); i++) {
		mock_chunk.data[column_ids[i]].Reference(chunk.data[i]);
	}
	mock_chunk.SetCardinality(chunk.size());
}

static bool CreateMockChunk(TableCatalogEntry &table, vector<column_t> &column_ids,
                            unordered_set<column_t> &desired_column_ids, DataChunk &chunk, DataChunk &mock_chunk) {
	idx_t found_columns = 0;
	// check whether the desired columns are present in the UPDATE clause
	for (column_t i = 0; i < column_ids.size(); i++) {
		if (desired_column_ids.find(column_ids[i]) != desired_column_ids.end()) {
			found_columns++;
		}
	}
	if (found_columns == 0) {
		// no columns were found: no need to check the constraint again
		return false;
	}
	if (found_columns != desired_column_ids.size()) {
		// FIXME: not all columns in UPDATE clause are present!
		// this should not be triggered at all as the binder should add these columns
		throw NotImplementedException(
		    "Not all columns required for the CHECK constraint are present in the UPDATED chunk!");
	}
	// construct a mock DataChunk
	auto types = table.GetTypes();
	CreateMockChunk(types, column_ids, chunk, mock_chunk);
	return true;
}

void DataTable::VerifyUpdateConstraints(TableCatalogEntry &table, DataChunk &chunk, vector<column_t> &column_ids) {
	for (auto &constraint : table.bound_constraints) {
		switch (constraint->type) {
		case ConstraintType::NOT_NULL: {
			auto &not_null = *reinterpret_cast<BoundNotNullConstraint *>(constraint.get());
			// check if the constraint is in the list of column_ids
			for (idx_t i = 0; i < column_ids.size(); i++) {
				if (column_ids[i] == not_null.index) {
					// found the column id: check the data in
					VerifyNotNullConstraint(table, chunk.data[i], chunk.size(), table.columns[not_null.index].name);
					break;
				}
			}
			break;
		}
		case ConstraintType::CHECK: {
			auto &check = *reinterpret_cast<BoundCheckConstraint *>(constraint.get());

			DataChunk mock_chunk;
			if (CreateMockChunk(table, column_ids, check.bound_columns, chunk, mock_chunk)) {
				VerifyCheckConstraint(table, *check.expression, mock_chunk);
			}
			break;
		}
		case ConstraintType::UNIQUE:
		case ConstraintType::FOREIGN_KEY:
			break;
		default:
			throw NotImplementedException("Constraint type not implemented!");
		}
	}
	// update should not be called for indexed columns!
	// instead update should have been rewritten to delete + update on higher layer
#ifdef DEBUG
	for (idx_t i = 0; i < info->indexes.size(); i++) {
		assert(!info->indexes[i]->IndexIsUpdated(column_ids));
	}
#endif
}

void DataTable::Update(TableCatalogEntry &table, ClientContext &context, Vector &row_ids, vector<column_t> &column_ids,
                       DataChunk &updates) {
	assert(row_ids.type == ROW_TYPE);

	updates.Verify();
	if (updates.size() == 0) {
		return;
	}

	// first verify that no constraints are violated
	VerifyUpdateConstraints(table, updates, column_ids);

	// now perform the actual update
	auto &transaction = Transaction::GetTransaction(context);

	updates.Normalify();
	row_ids.Normalify(updates.size());
	auto first_id = FlatVector::GetValue<row_t>(row_ids, 0);
	if (first_id >= MAX_ROW_ID) {
		// update is in transaction-local storage: push update into local storage
		transaction.storage.Update(this, row_ids, column_ids, updates);
		return;
	}

	for (idx_t i = 0; i < column_ids.size(); i++) {
		auto column = column_ids[i];
		assert(column != COLUMN_IDENTIFIER_ROW_ID);

		columns[column]->Update(transaction, updates.data[i], row_ids, updates.size());
	}
}

//===--------------------------------------------------------------------===//
// Create Index Scan
//===--------------------------------------------------------------------===//
void DataTable::InitializeCreateIndexScan(CreateIndexScanState &state, vector<column_t> column_ids) {
	// we grab the append lock to make sure nothing is appended until AFTER we finish the index scan
	state.append_lock = unique_lock<mutex>(append_lock);
	// get a read lock on the VersionManagers to prevent any further deletions
	state.locks.push_back(persistent_manager->lock.GetSharedLock());
	state.locks.push_back(transient_manager->lock.GetSharedLock());

	InitializeScan(state, column_ids);
}

void DataTable::CreateIndexScan(CreateIndexScanState &state, DataChunk &result) {
	// scan the persistent segments
	if (ScanCreateIndex(state, result, state.current_persistent_row, state.max_persistent_row, 0)) {
		return;
	}
	// scan the transient segments
	if (ScanCreateIndex(state, result, state.current_transient_row, state.max_transient_row,
	                    state.max_persistent_row)) {
		return;
	}
}

bool DataTable::ScanCreateIndex(CreateIndexScanState &state, DataChunk &result, idx_t &current_row, idx_t max_row,
                                idx_t base_row) {
	if (current_row >= max_row) {
		return false;
	}
	idx_t count = std::min((idx_t)STANDARD_VECTOR_SIZE, max_row - current_row);

	// scan the base columns to fetch the actual data
	// note that we insert all data into the index, even if it is marked as deleted
	// FIXME: tuples that are already "cleaned up" do not need to be inserted into the index!
	for (idx_t i = 0; i < state.column_ids.size(); i++) {
		auto column = state.column_ids[i];
		if (column == COLUMN_IDENTIFIER_ROW_ID) {
			// scan row id
			assert(result.data[i].type == ROW_TYPE);
			result.data[i].Sequence(base_row + current_row, 1);
		} else {
			// scan actual base column
			columns[column]->IndexScan(state.column_scans[i], result.data[i]);
		}
	}
	result.SetCardinality(count);

	current_row += STANDARD_VECTOR_SIZE;
	return count > 0;
}

void DataTable::AddIndex(unique_ptr<Index> index, vector<unique_ptr<Expression>> &expressions) {
	DataChunk result;
	result.Initialize(index->types);

	DataChunk intermediate;
	vector<TypeId> intermediate_types;
	auto column_ids = index->column_ids;
	column_ids.push_back(COLUMN_IDENTIFIER_ROW_ID);
	for (auto &id : index->column_ids) {
		intermediate_types.push_back(types[id]);
	}
	intermediate_types.push_back(ROW_TYPE);
	intermediate.Initialize(intermediate_types);

	// initialize an index scan
	CreateIndexScanState state;
	InitializeCreateIndexScan(state, column_ids);

	if (!is_root) {
		throw TransactionException("Transaction conflict: cannot add an index to a table that has been altered!");
	}

	// now start incrementally building the index
	IndexLock lock;
	index->InitializeLock(lock);
	ExpressionExecutor executor(expressions);
	while (true) {
		intermediate.Reset();
		// scan a new chunk from the table to index
		CreateIndexScan(state, intermediate);
		if (intermediate.size() == 0) {
			// finished scanning for index creation
			// release all locks
			break;
		}
		// resolve the expressions for this chunk
		executor.Execute(intermediate, result);

		// insert into the index
		if (!index->Insert(lock, result, intermediate.data[intermediate.column_count() - 1])) {
			throw ConstraintException("Cant create unique index, table contains duplicate data on indexed column(s)");
		}
	}
	info->indexes.push_back(move(index));
}

void DataTable::UpdateColumnCards(Transaction &transaction) {
	DataChunk result;
	result.Initialize(this->types);
	vector<idx_t> columns_ids;
	for (idx_t i = 0; i < this->types.size(); i++) {
		columns_ids.push_back(i);
	}
	// initialize an cardinality scan
	TableScanState state;
	InitializeScan(transaction, state, columns_ids);
	// now start incrementally building the column cardinality
	idx_t result_count = 0;
	unordered_map<idx_t, vector<TableFilter>> empty_table_filter;
	for (idx_t i = 0; i < columns_ids.size(); i++) {
		auto hyper_obj = make_unique<HyperLogLog>();
		info->column_card_objs.push_back(move(hyper_obj));
	}
	do {
		result.Reset();
		// scan a new chunk from the table to index
		this->Scan(transaction, result, state, empty_table_filter);
		result_count = result.size();
		if (result_count == 0)
			break;
		for (idx_t col = 0; col < columns_ids.size(); col++) {
			for (idx_t row = 0; row < result_count; row++) {
				auto result_tuple = result.GetValue(col, row).ToString();
				auto pCUC_result_tuple = (unsigned char *)result_tuple.c_str();
				this->info->column_card_objs[col]->Add(pCUC_result_tuple, result_tuple.length());
			}
		}
	} while (result_count != 0);
	// get final count
	info->column_cards.clear();
	info->column_cards.resize(columns_ids.size(), 0);
	for (column_t i = 0; i < columns_ids.size(); i++) {
		info->column_cards[i] = info->column_card_objs[i]->Count();
	}
}

void DataTable::AddRAI(unique_ptr<RAI> rai) const {
	info->rais.push_back(move(rai));
}

void DataTable::AppendRAIColumn(column_t oid) {
	column_t columns_count = columns.size();
	assert(columns_count == oid);
	auto column = make_shared<ColumnData>(*storage.buffer_manager, *info);
	column->type = TypeId::INT64;
	column->column_idx = oid;
	columns.push_back(move(column));
}

string DataTable::GetRAIsInfo() const {
	//! example:: rai: order_rai(o_custkey: Customer.c_custkey, o_orderkey: Lineitem.l_orderkey)
	//! param:&table, column_ids, referenced_tables, referenced_columns
	if (info->rais.empty())
		return "false";
	string result = "rai: ";
	for (auto &rai : info->rais) {
		result += rai->ToString();
	}
	return result;
}
