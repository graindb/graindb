//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/optimizer/adjacency_compression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/planner/logical_operator.hpp"
#include "duckdb/planner/operator/logical_comparison_join.hpp"
#include "duckdb/planner/operator/logical_get.hpp"

namespace duckdb {

class Optimizer;

class AdjacencyCompression {
public:
	unique_ptr<LogicalOperator> Optimize(unique_ptr<LogicalOperator> op);

private:
	unique_ptr<LogicalOperator> Compress(unique_ptr<LogicalOperator> op);
};

} // namespace duckdb
