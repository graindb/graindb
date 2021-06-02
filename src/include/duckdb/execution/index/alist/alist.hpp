//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/execution/index/alist/alist.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/constants.hpp"
#include "duckdb/storage/index.hpp"
#include "duckdb/common/types/chunk_collection.hpp"
#include <stack>

namespace duckdb {

struct AListIndexScanState : public IndexScanState {
	AListIndexScanState(vector<column_t> column_ids) : IndexScanState(column_ids), finished(false) {
	}

	Value values[2];
	ExpressionType expression_types[2];
	bool finished;
};

struct VNode {
	int64_t value;
	int8_t level;

	VNode(int64_t value, int8_t level) : value(value), level(level) {
	}
};

class AList : public Index {
public:
	AList(DataTable &table, vector<column_t> column_ids, vector<unique_ptr<Expression>> unbound_expressions,
	      bool is_unique = false);
	~AList();

	bool is_unique;
	unordered_map<int64_t, unique_ptr<vector<int64_t>>> adjacency_map;

public:
	//! Initialize a scan on the index with the given expression and column ids
	//! to fetch from the base table for a single predicate
	unique_ptr<IndexScanState> InitializeScanSinglePredicate(Transaction &transaction, vector<column_t> column_ids,
	                                                         Value value, ExpressionType expression_type) override;
	//! Initialize a scan on the index with the given expression and column ids
	//! to fetch from the base table for two predicates
	unique_ptr<IndexScanState> InitializeScanTwoPredicates(Transaction &transaction, vector<column_t> column_ids,
	                                                       Value low_value, ExpressionType low_expression_type,
	                                                       Value high_value,
	                                                       ExpressionType high_expression_type) override;
	//! Perform a lookup on the index
	void Scan(Transaction &transaction, TableIndexScanState &state, DataChunk &result) override;
	//! Append entries to the index
	bool Append(IndexLock &lock, DataChunk &entries, Vector &row_identifiers) override;
	//! Verify that data can be appended to the index
	void VerifyAppend(DataChunk &chunk) override;
	//! Delete entries in the index
	void Delete(IndexLock &lock, DataChunk &entries, Vector &row_identifiers) override;
	//! Insert data into the index.
	bool Insert(IndexLock &lock, DataChunk &data, Vector &row_ids) override;

	void FillChunk(int64_t id, ChunkCollection &result_chunks, vector<TypeId> &result_types, column_t dest_id_column,
	               vector<Value> &anchor_values);
	void StackPush(int64_t id, std::stack<VNode> &result, int8_t level);

private:
	DataChunk expression_result;
};
} // namespace duckdb
