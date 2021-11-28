#include "duckdb/execution/operator/scan/physical_table_scan.hpp"

#include "duckdb/catalog/catalog_entry/table_catalog_entry.hpp"
#include "duckdb/planner/expression/bound_conjunction_expression.hpp"
#include "duckdb/transaction/transaction.hpp"

#include <iostream>
#include <utility>

using namespace duckdb;
using namespace std;

class PhysicalTableScanOperatorState : public PhysicalOperatorState {
public:
	PhysicalTableScanOperatorState(Expression &expr)
	    : PhysicalOperatorState(nullptr), initialized(false), executor(expr) {
	}
	PhysicalTableScanOperatorState() : PhysicalOperatorState(nullptr), initialized(false) {
	}
	//! Whether or not the scan has been initialized
	bool initialized;
	//! The current position in the scan
	TableScanState scan_offset;
	//! Execute filters inside the table
	ExpressionExecutor executor;
	TableIndexScanState index_state;
};

PhysicalTableScan::PhysicalTableScan(LogicalOperator &op, TableCatalogEntry &tableref, idx_t table_index,
                                     DataTable &table, vector<column_t> column_ids,
                                     vector<unique_ptr<Expression>> filter,
                                     unordered_map<idx_t, vector<TableFilter>> table_filters)
    : PhysicalOperator(PhysicalOperatorType::SEQ_SCAN, op.types), tableref(tableref), table_index(table_index),
      table(table), column_ids(move(column_ids)), table_filters(move(table_filters)), rows_filter(nullptr),
      rows_count(-1) {
	if (filter.size() > 1) {
		//! create a big AND out of the expressions
		auto conjunction = make_unique<BoundConjunctionExpression>(ExpressionType::CONJUNCTION_AND);
		for (auto &expr : filter) {
			conjunction->children.push_back(move(expr));
		}
		expression = move(conjunction);
	} else if (filter.size() == 1) {
		expression = move(filter[0]);
	}
}

bool PhysicalTableScan::PushdownZoneFilter(idx_t table_index_, const shared_ptr<bitmask_vector> &row_bitmask_,
                                           const shared_ptr<bitmask_vector> &zone_bitmask_) {
	if (this->table_index == table_index_) {
		if (this->row_bitmask) {
			auto &result_row_bitmask = *row_bitmask;
			auto &input_row_bitmask = *row_bitmask_;
			auto &result_zone_bitmask = *zone_bitmask;
			auto &input_zone_bitmask = *zone_bitmask_;
			for (idx_t i = 0; i < zone_bitmask_->size(); i++) {
				result_zone_bitmask[i] = result_zone_bitmask[i] & input_zone_bitmask[i];
			}
			auto zone_filter_index = 0;
			for (idx_t i = 0; i < result_zone_bitmask.size(); i++) {
				auto zone_count = result_zone_bitmask[i] * STANDARD_VECTOR_SIZE;
				for (idx_t j = 0; j < zone_count; j++) {
					auto current_index = i * STANDARD_VECTOR_SIZE + j;
					result_row_bitmask[current_index] =
					    result_row_bitmask[current_index] & input_row_bitmask[current_index];
				}
				zone_filter_index += STANDARD_VECTOR_SIZE;
			}
		} else {
			this->row_bitmask = row_bitmask_;
			this->zone_bitmask = zone_bitmask_;
		}
		return true;
	}
	return false;
}

bool PhysicalTableScan::PushdownRowsFilter(idx_t table_index_, const shared_ptr<rows_vector> &rows_filter_,
                                           idx_t count) {
	if (this->table_index == table_index_) {
		if (this->rows_count <= 0 || (row_t)count < this->rows_count) {
			this->rows_filter = rows_filter_;
			this->rows_count = count;
		}
		return true;
	}
	return false;
}

void PhysicalTableScan::PerformSeqScan(DataChunk &chunk, PhysicalOperatorState *state_, Transaction &transaction) {
	auto state = reinterpret_cast<PhysicalTableScanOperatorState *>(state_);

	if (!state->initialized) {
		if (rows_count != -1) {
			table.InitializeScan(state->scan_offset, column_ids, rows_filter, rows_count, &table_filters);
			state->initialized = true;
		} else if (row_bitmask) {
			table.InitializeScan(state->scan_offset, column_ids, row_bitmask, zone_bitmask, &table_filters);
			state->initialized = true;
		} else {
			table.InitializeScan(transaction, state->scan_offset, column_ids, &table_filters);
			state->initialized = true;
		}
	}
	table.Scan(transaction, chunk, state->scan_offset, table_filters);
}

template <class T> static void Lookup(ColumnData &column, row_t *row_ids, Vector &result, idx_t count) {
	auto result_data = FlatVector::GetData(result);
	auto type_size = sizeof(T);
	idx_t s_size = column.data.nodes[0].node->count;
	for (idx_t i = 0; i < count; i++) {
		row_t row_id = row_ids[i];
		idx_t s_index = row_id / s_size;
		idx_t s_offset = row_id % s_size;
		idx_t vector_index = s_offset / STANDARD_VECTOR_SIZE;
		idx_t id_in_vector = s_offset - vector_index * STANDARD_VECTOR_SIZE;
		// get segment buffer
		auto transient_segment = (TransientSegment *)column.data.nodes[s_index].node;
		auto numeric_segment = (NumericSegment *)transient_segment->data.get();
		assert(vector_index < numeric_segment->max_vector_count);
		auto block_entry = transient_segment->manager.blocks.find(numeric_segment->block_id);
		if (block_entry == transient_segment->manager.blocks.end()) {
			continue;
		}
		auto s_base = block_entry->second->buffer->buffer;
		auto s_data =
		    s_base + (vector_index * (sizeof(nullmask_t) + type_size * STANDARD_VECTOR_SIZE) + sizeof(nullmask_t));
		memcpy(result_data + (i * type_size), s_data + (id_in_vector * type_size), type_size);
	}
}

void PhysicalTableScan::PerformLookup(DataChunk &chunk, PhysicalOperatorState *state_, SelectionVector *sel,
                                      Vector *rid_vector, DataChunk *rai_chunk, Transaction &transaction) {
	auto state = reinterpret_cast<PhysicalTableScanOperatorState *>(state_);
	auto fetch_count = rai_chunk->size();
	if (fetch_count == 0) {
		return;
	}
	// perform lookups
	auto row_ids = FlatVector::GetData<row_t>(*rid_vector);
	vector<column_t> rai_columns;
	chunk.SetCardinality(fetch_count);
	for (idx_t col_idx = 0; col_idx < column_ids.size(); col_idx++) {
		auto col = column_ids[col_idx];
		if (col == COLUMN_IDENTIFIER_ROW_ID) {
			chunk.data[col_idx].Reference(*rid_vector);
		} else {
			auto column = table.GetColumn(col);
			switch (column->type) {
			case TypeId::INT8: {
				Lookup<int8_t>(*column.get(), row_ids, chunk.data[col_idx], fetch_count);
				break;
			}
			case TypeId::UINT8: {
				Lookup<uint8_t>(*column.get(), row_ids, chunk.data[col_idx], fetch_count);
				break;
			}
			case TypeId::INT16: {
				Lookup<int16_t>(*column.get(), row_ids, chunk.data[col_idx], fetch_count);
				break;
			}
			case TypeId::HASH:
			case TypeId::UINT16: {
				Lookup<uint16_t>(*column.get(), row_ids, chunk.data[col_idx], fetch_count);
				break;
			}
			case TypeId::INT32: {
				Lookup<int32_t>(*column.get(), row_ids, chunk.data[col_idx], fetch_count);
				break;
			}
			case TypeId::UINT32: {
				Lookup<uint32_t>(*column.get(), row_ids, chunk.data[col_idx], fetch_count);
				break;
			}
			case TypeId::TIMESTAMP:
			case TypeId::INT64: {
				Lookup<int64_t>(*column.get(), row_ids, chunk.data[col_idx], fetch_count);
				break;
			}
			case TypeId::UINT64: {
				Lookup<uint64_t>(*column.get(), row_ids, chunk.data[col_idx], fetch_count);
				break;
			}
			case TypeId::FLOAT: {
				Lookup<float_t>(*column.get(), row_ids, chunk.data[col_idx], fetch_count);
				break;
			}
			case TypeId::DOUBLE: {
				Lookup<double_t>(*column.get(), row_ids, chunk.data[col_idx], fetch_count);
				break;
			}
			case TypeId::POINTER: {
				Lookup<uintptr_t>(*column.get(), row_ids, chunk.data[col_idx], fetch_count);
				break;
			}
			default: {
				table.Fetch(transaction, chunk, column_ids, *rid_vector, fetch_count, state->index_state);
			}
			}
		}
	}
	// filter
	SelectionVector filter_sel(fetch_count);
	auto result_count = fetch_count;
	if (table_filters.size() > 0) {
		result_count = state->executor.SelectExpression(chunk, filter_sel);
	}
#if ENABLE_PROFILING
	lookup_size += result_count;
#endif
	if (result_count == fetch_count) {
		// nothing was filtered: skip adding any selection vectors
		return;
	}
	// slice
	chunk.Slice(filter_sel, result_count);
	auto sel_data = sel->Slice(filter_sel, result_count);
	sel->Initialize(move(sel_data));
}

void PhysicalTableScan::GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
                                         SelectionVector *sel, Vector *rid_vector, DataChunk *rai_chunk) {
	if (column_ids.empty()) {
		return;
	}
	auto &transaction = context.ActiveTransaction();
	if (rid_vector == nullptr) {
#if ENABLE_PROFILING
		seq_scan = true;
#endif
		PerformSeqScan(chunk, state_, transaction);
	} else {
#if ENABLE_PROFILING
		seq_scan = false;
#endif
		PerformLookup(chunk, state_, sel, rid_vector, rai_chunk, transaction);
	}
}

string PhysicalTableScan::ExtraRenderInformation() const {
	string result = "";
	if (expression) {
		result += tableref.name + " " + expression->ToString();
	} else {
		result += tableref.name;
	}
	result += "(" + to_string(table_index) + ")";
	result += "[";
	for (auto &id : column_ids) {
		if (id == COLUMN_IDENTIFIER_ROW_ID) {
			result += "rowid,";
		} else {
			result += tableref.columns[id].name + ",";
		}
	}
	result = result.substr(0, result.size() - 1);
	result += "]";
//	if (seq_scan) {
//		if (rows_count >= 0) {
//			result += "\nROWS_FILTER(" + to_string(rows_count) + "/" + to_string(table.info->cardinality) + ")";
//		} else if (row_bitmask) {
//			result += "\nROWS_BITMASK(" + to_string(row_bitmask->count()) + "/" + to_string(row_bitmask->size()) + ")";
//			result +=
//			    "\nZONE_BITMASK(" + to_string(zone_bitmask->count()) + "/" + to_string(zone_bitmask->size()) + ")";
//		}
//	} else {
//		result += "\nLOOKUP(" + to_string(lookup_size) + "/" + to_string(table.info->cardinality) + ")";
//	}

	return result;
}

unique_ptr<PhysicalOperatorState> PhysicalTableScan::GetOperatorState() {
	if (expression) {
		return make_unique<PhysicalTableScanOperatorState>(*expression);
	} else {
		return make_unique<PhysicalTableScanOperatorState>();
	}
}
