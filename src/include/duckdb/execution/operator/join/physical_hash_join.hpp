//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/execution/operator/join/physical_hash_join.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/types/chunk_collection.hpp"
#include "duckdb/execution/join_hashtable.hpp"
#include "duckdb/execution/operator/join/physical_comparison_join.hpp"
#include "duckdb/execution/physical_operator.hpp"
#include "duckdb/execution/rai_hashtable.hpp"
#include "duckdb/planner/operator/logical_join.hpp"

namespace duckdb {

//! PhysicalHashJoin represents a hash loop join between two tables
class PhysicalHashJoin : public PhysicalComparisonJoin {
public:
	PhysicalHashJoin(ClientContext &context, LogicalOperator &op, unique_ptr<PhysicalOperator> left,
	                 unique_ptr<PhysicalOperator> right, vector<JoinCondition> cond, JoinType join_type,
	                 vector<idx_t> left_projection_map, vector<idx_t> right_projection_map);
	PhysicalHashJoin(ClientContext &context, LogicalOperator &op, unique_ptr<PhysicalOperator> left,
	                 unique_ptr<PhysicalOperator> right, vector<JoinCondition> cond, JoinType join_type);

	unique_ptr<JoinHashTable> hash_table;
	vector<idx_t> right_projection_map;
	idx_t build_time = 0;
	idx_t probe_time = 0;
	idx_t ht_expr_time = 0;
	idx_t ht_probe_time = 0;
	idx_t ht_next_time = 0;
	idx_t build_finalize_time = 0;

public:
	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;
	unique_ptr<PhysicalOperatorState> GetOperatorState() override;
	string ExtraRenderInformation() const override;

private:
	void BuildHashTable(ClientContext &context, PhysicalOperatorState *state_);
	void ProbeHashTable(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_);
};

} // namespace duckdb
