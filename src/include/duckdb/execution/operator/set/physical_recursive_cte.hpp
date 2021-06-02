//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/execution/operator/set/physical_recursive_cte.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/types/chunk_collection.hpp"
#include "duckdb/execution/physical_operator.hpp"

namespace duckdb {
class PhysicalRecursiveCTE : public PhysicalOperator {
public:
	PhysicalRecursiveCTE(LogicalOperator &op, bool union_all, unique_ptr<PhysicalOperator> top,
	                     unique_ptr<PhysicalOperator> bottom);

	bool union_all;
	std::shared_ptr<ChunkCollection> working_table;
	ChunkCollection intermediate_table;

public:
	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;
	unique_ptr<PhysicalOperatorState> GetOperatorState() override;

private:
	//! Probe Hash Table and eliminate duplicate rows
	idx_t ProbeHT(DataChunk &chunk, PhysicalOperatorState *state);
};

}; // namespace duckdb
