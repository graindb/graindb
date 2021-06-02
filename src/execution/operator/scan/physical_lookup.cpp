#include "duckdb/execution/operator/scan/physical_lookup.hpp"

#include "duckdb/catalog/catalog_entry/table_catalog_entry.hpp"
#include "duckdb/main/client_context.hpp"
#include "duckdb/main/database.hpp"
#include "duckdb/storage/buffer_manager.hpp"
#include "duckdb/storage/numeric_segment.hpp"
#include "duckdb/storage/storage_manager.hpp"
#include "duckdb/storage/table/transient_segment.hpp"
#include "duckdb/storage/uncompressed_segment.hpp"
#include "duckdb/transaction/transaction.hpp"

#include <iostream>

using namespace duckdb;
using namespace std;

PhysicalLookup::PhysicalLookup(LogicalOperator &op, TableCatalogEntry &tableref, idx_t table_index, DataTable &table,
                               vector<column_t> &column_ids, vector<unique_ptr<Expression>> filter,
                               unordered_map<idx_t, vector<TableFilter>> table_filters)
    : PhysicalOperator(PhysicalOperatorType::LOOKUP, op.types), tableref(tableref), table_index(table_index),
      table(table), column_ids(column_ids), table_filters(move(table_filters)) {
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
	for (column_t i = 0; i < column_ids.size(); i++) {
		unordered_map<idx_t, data_ptr_t> s_ptrs;
		segment_ptrs_map.push_back(s_ptrs);
	}
}

template <class T>
void inline PhysicalLookup::Lookup(ClientContext &context, ColumnData &column, row_t *row_ids, Vector &result,
                                   idx_t count, unordered_map<idx_t, data_ptr_t> &segment_ptrs, idx_t type_size) {
	auto result_data = FlatVector::GetData(result);
	idx_t s_size = column.data.nodes[0].node->count;
	for (idx_t i = 0; i < count; i++) {
		row_t row_id = row_ids[i];
		idx_t s_index = row_id / s_size;
		idx_t s_offset = row_id % s_size;
		idx_t vector_index = s_offset / STANDARD_VECTOR_SIZE;
		idx_t id_in_vector = s_offset - vector_index * STANDARD_VECTOR_SIZE;
		if (segment_ptrs.find(s_index) == segment_ptrs.end()) {
			// get segment buffer
			auto transient_segment = (TransientSegment *)column.data.nodes[s_index].node;
			auto numeric_segment = (NumericSegment *)transient_segment->data.get();
			assert(vector_index < numeric_segment->max_vector_count);
			auto block_entry = transient_segment->manager.blocks.find(numeric_segment->block_id);
			if (block_entry == transient_segment->manager.blocks.end()) {
				continue;
			}
			segment_ptrs[s_index] = block_entry->second->buffer->buffer;
		}
		auto s_data = segment_ptrs[s_index] +
		              (vector_index * (sizeof(nullmask_t) + type_size * STANDARD_VECTOR_SIZE) + sizeof(nullmask_t));
		memcpy(result_data + (i * type_size), s_data + (id_in_vector * type_size), type_size);
	}
}

void PhysicalLookup::GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
                                      SelectionVector *sel, Vector *rid_vector, DataChunk *rai_chunk) {
	auto state = reinterpret_cast<PhysicalLookupOperatorState *>(state_);
	auto &transaction = context.ActiveTransaction();
	auto reference_column_idx = rai_chunk->column_count() - 1;
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
		if (col == COLUMN_IDENTIFIER_ROW_ID || col == table.types.size()) {
			chunk.data[col_idx].Reference(*rid_vector);
		} else if (col == table.types.size() + 2) {
			chunk.data[col_idx].Reference(rai_chunk->data[reference_column_idx]);
		} else {
			auto column = table.GetColumn(col);
			switch (column->type) {
			case TypeId::INT8: {
				Lookup<int8_t>(context, *column.get(), row_ids, chunk.data[col_idx], fetch_count,
				               segment_ptrs_map[col_idx]);
				break;
			}
			case TypeId::UINT8: {
				Lookup<uint8_t>(context, *column.get(), row_ids, chunk.data[col_idx], fetch_count,
				                segment_ptrs_map[col_idx]);
				break;
			}
			case TypeId::INT16: {
				Lookup<int16_t>(context, *column.get(), row_ids, chunk.data[col_idx], fetch_count,
				                segment_ptrs_map[col_idx]);
				break;
			}
			case TypeId::HASH:
			case TypeId::UINT16: {
				Lookup<uint16_t>(context, *column.get(), row_ids, chunk.data[col_idx], fetch_count,
				                 segment_ptrs_map[col_idx]);
				break;
			}
			case TypeId::INT32: {
				Lookup<int32_t>(context, *column.get(), row_ids, chunk.data[col_idx], fetch_count,
				                segment_ptrs_map[col_idx]);
				break;
			}
			case TypeId::UINT32: {
				Lookup<uint32_t>(context, *column.get(), row_ids, chunk.data[col_idx], fetch_count,
				                 segment_ptrs_map[col_idx]);
				break;
			}
			case TypeId::TIMESTAMP:
			case TypeId::INT64: {
				Lookup<int64_t>(context, *column.get(), row_ids, chunk.data[col_idx], fetch_count,
				                segment_ptrs_map[col_idx]);
				break;
			}
			case TypeId::UINT64: {
				Lookup<uint64_t>(context, *column.get(), row_ids, chunk.data[col_idx], fetch_count,
				                 segment_ptrs_map[col_idx]);
				break;
			}
			case TypeId::FLOAT: {
				Lookup<float_t>(context, *column.get(), row_ids, chunk.data[col_idx], fetch_count,
				                segment_ptrs_map[col_idx]);
				break;
			}
			case TypeId::DOUBLE: {
				Lookup<double_t>(context, *column.get(), row_ids, chunk.data[col_idx], fetch_count,
				                 segment_ptrs_map[col_idx]);
				break;
			}
			case TypeId::POINTER: {
				Lookup<uintptr_t>(context, *column.get(), row_ids, chunk.data[col_idx], fetch_count,
				                  segment_ptrs_map[col_idx]);
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
	// reference
	//	auto rai_column_idx = column_ids.size();
	//	for (auto &col : rai_columns) {
	//		if (col == table.types.size()) {
	//			chunk.data[rai_column_idx++].Reference(*rid_vector);
	//		}
	//		if (col == table.types.size() + 2) {
	//			chunk.data[rai_column_idx++].Reference(rai_chunk->data[reference_column_idx]);
	//		}
	//	}
	if (result_count == fetch_count) {
		// nothing was filtered: skip adding any selection vectors
		return;
	}
	// slice
	chunk.Slice(filter_sel, result_count);
	//	rai_chunk->Slice(filter_sel, result_count);
	auto sel_data = sel->Slice(filter_sel, result_count);
	sel->Initialize(move(sel_data));
}

string PhysicalLookup::ExtraRenderInformation() const {
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

	return result;
}

unique_ptr<PhysicalOperatorState> PhysicalLookup::GetOperatorState() {
	if (table_filters.size() > 0) {
		return make_unique<PhysicalLookupOperatorState>(*expression);
	}
	return make_unique<PhysicalLookupOperatorState>();
}
