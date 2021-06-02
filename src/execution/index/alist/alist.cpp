#include "duckdb/execution/index/alist/alist.hpp"

#include "duckdb/catalog/catalog_entry/table_catalog_entry.hpp"
#include "duckdb/common/constants.hpp"
#include "duckdb/common/vector_operations/vector_operations.hpp"
#include "duckdb/storage/data_table.hpp"

using namespace duckdb;
using namespace std;

AList::AList(DataTable &table, vector<column_t> column_ids, vector<unique_ptr<Expression>> unbound_expressions,
             bool is_unique)
    : Index(IndexType::ALIST, table, column_ids, move(unbound_expressions)), is_unique(is_unique) {
	if (column_ids.size() != 2) {
		throw Exception("AList initialize error: size of column ids is not 2.");
	}
	assert(types[0] == TypeId::INT64 && types[1] == TypeId::INT64);
	expression_result.Initialize(types);
}

AList::~AList() {
}

unique_ptr<IndexScanState> AList::InitializeScanSinglePredicate(Transaction &transaction, vector<column_t> column_ids,
                                                                Value value, ExpressionType expression_type) {
	auto result = make_unique<AListIndexScanState>(column_ids);
	result->values[0] = value;
	result->expression_types[0] = expression_type;
	return move(result);
}

unique_ptr<IndexScanState> AList::InitializeScanTwoPredicates(Transaction &transaction, vector<column_t> column_ids,
                                                              Value low_value, ExpressionType low_expression_type,
                                                              Value high_value, ExpressionType high_expression_type) {
	auto result = make_unique<AListIndexScanState>(column_ids);
	result->values[0] = low_value;
	result->values[1] = high_value;
	result->expression_types[0] = low_expression_type;
	result->expression_types[1] = high_expression_type;
	return move(result);
}

bool AList::Insert(IndexLock &lock, DataChunk &data, Vector &row_ids) {
	//	auto src_data = (int64_t *)data.data[0].GetData();
	//	auto dest_data = (int64_t *)data.data[1].GetData();
	//	for (idx_t i = 0; i < data.size(); i++) {
	//		if (adjacency_map.find(src_data[i]) == adjacency_map.end()) {
	//			auto src_vec = make_unique<vector<int64_t>>();
	//			src_vec->push_back(dest_data[i]);
	//			adjacency_map[src_data[i]] = move(src_vec);
	//		} else {
	//			adjacency_map[src_data[i]]->push_back(dest_data[i]);
	//		}
	//	}
	//	return true;
}

void AList::Scan(Transaction &transaction, TableIndexScanState &table_state, DataChunk &result) {
	//	auto state = (AListIndexScanState *)table_state.index_state.get();
	//	assert(state->values[0].type == types[0]);
	//
	//	if (state->values[1].is_null) {
	//		lock_guard<mutex> l(lock);
	//		switch (state->expression_types[0]) {
	//		case ExpressionType::COMPARE_EQUAL: {
	//			auto v = state->values[0].value_.hash;
	//			if (adjacency_map.find(v) == adjacency_map.end() || adjacency_map[v]->empty()) {
	//				return;
	//			}
	//			 todo: if count is larger than VECTOR size
	//			idx_t count = adjacency_map[v]->size();
	//			auto src_data = (uint64_t *)result.data[0].GetData();
	//			auto dest_data = (uint64_t *)result.data[1].GetData();
	//			VectorOperations::Exec(nullptr, count, [&](idx_t i, idx_t k) {
	//				src_data[i] = v;
	//				dest_data[i] = adjacency_map[v]->operator[](i);
	//			});
	//						result.data[0].count = count;
	//						result.data[1].count = count;
	//			break;
	//		}
	//		default:
	//			throw NotImplementedException("AList scan on non_equal not implemented");
	//		}
	//	} else {
	//		throw NotImplementedException("AList scan on two predicates not implemented");
	//	}
}

void AList::FillChunk(int64_t id, ChunkCollection &result_chunks, vector<TypeId> &result_types, column_t dest_id_column,
                      vector<Value> &anchor_values) {
	//	if (adjacency_map.find(id) == adjacency_map.end()) {
	//		return;
	//	}
	//	idx_t vector_size = adjacency_map[id]->size();
	//	auto chunk_num = (vector_size + STANDARD_VECTOR_SIZE - 1) / STANDARD_VECTOR_SIZE;
	//	idx_t chunk_idx = 0;
	//	while (chunk_idx < chunk_num) {
	//		auto count = std::min<idx_t>(STANDARD_VECTOR_SIZE, vector_size - (chunk_idx * STANDARD_VECTOR_SIZE));
	//		DataChunk new_chunk;
	//		new_chunk.SetCardinality(count, nullptr);
	//		new_chunk.Initialize(result_types);
	//		assert(anchor_values.size() == dest_id_column);
	//		for (column_t i = 0; i < dest_id_column; i++) {
	//			new_chunk.data[i].Reference(anchor_values[i]);
	//		}
	//		auto dest_id_data = (int64_t *)new_chunk.data[dest_id_column].GetData();
	//		memcpy(dest_id_data, &adjacency_map[id]->operator[](chunk_idx *STANDARD_VECTOR_SIZE), count *
	//sizeof(int64_t)); 		result_chunks.Append(new_chunk); 		chunk_idx++;
	//	}
}

void AList::StackPush(int64_t id, std::stack<VNode> &result, int8_t level) {
	if (adjacency_map.find(id) != adjacency_map.end()) {
		for (idx_t i = 0; i < adjacency_map[id]->size(); i++) {
			VNode v_node(adjacency_map[id]->operator[](i), level);
			result.push(v_node);
		}
	}
}

bool AList::Append(IndexLock &lock, DataChunk &entries, Vector &row_identifiers) {
	//	ExecuteExpressions(entries, expression_result);
	//	expression_result.Reset();
	//	table.table_catalog_entry_->from_tbl->storage->Search(entries.data[0], expression_result.data[0]);
	//	table.table_catalog_entry_->to_tbl->storage->Search(entries.data[1], expression_result.data[1]);
	return Insert(lock, expression_result, row_identifiers);
}

void AList::VerifyAppend(DataChunk &chunk) {
	// todo: AList verify append
}

void AList::Delete(IndexLock &lock, DataChunk &entries, Vector &row_identifiers) {
	ExecuteExpressions(entries, expression_result);
	auto src_data = (uint64_t *)expression_result.data[0].GetData();
	for (idx_t i = 0; i < expression_result.size(); i++) {
		adjacency_map[src_data[i]]->clear();
	}
}
