//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/planner/logical_operator.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/catalog/catalog.hpp"
#include "duckdb/common/common.hpp"
#include "duckdb/common/enums/logical_operator_type.hpp"
#include "duckdb/planner/column_binding.hpp"
#include "duckdb/planner/expression.hpp"
#include "duckdb/planner/logical_operator_visitor.hpp"

#include <functional>

namespace duckdb {

// nested loop join, hash join, adjacency join, default join, scan, lookup
enum class OpMark : int8_t {
	NO_OP = 0,
	NLJ = 1,
	HASH_JOIN = 2,
	SIP_JOIN = 3,
	JOIN = 4,
	MERGED_SIP_JOIN = 5,
	SCAN = 6,
	UNION = 7,
	ADAPTIVE_SIP_JOIN = 8,
	ADAPTIVE_MERGE_SIP_JOIN = 9
};

//! LogicalOperator is the base class of the logical operators present in the
//! logical query tree
class LogicalOperator {
public:
	LogicalOperator(LogicalOperatorType type) : type(type), op_mark(OpMark::NO_OP) {
	}
	LogicalOperator(LogicalOperatorType type, vector<unique_ptr<Expression>> expressions)
	    : type(type), op_mark(OpMark::NO_OP), expressions(move(expressions)) {
	}
	virtual ~LogicalOperator() {
	}

	//! The type of the logical operator
	LogicalOperatorType type;
	OpMark op_mark;
	//! The set of children of the operator
	vector<unique_ptr<LogicalOperator>> children;
	//! The set of expressions contained within the operator, if any
	vector<unique_ptr<Expression>> expressions;
	//! The types returned by this logical operator. Set by calling LogicalOperator::ResolveTypes.
	vector<TypeId> types;

public:
	virtual vector<ColumnBinding> GetColumnBindings() {
		return {};
	}
	static vector<ColumnBinding> GenerateColumnBindings(idx_t table_idx, idx_t column_count);
	static vector<TypeId> MapTypes(vector<TypeId> types, vector<idx_t> projection_map);
	static vector<ColumnBinding> MapBindings(vector<ColumnBinding> types, vector<idx_t> projection_map);

	//! Resolve the types of the logical operator and its children
	void ResolveOperatorTypes();

	virtual string ParamsToString() const;
	virtual string ToString(idx_t depth = 0) const;
	string ToJSON() const;
	void Print();

	void AddChild(unique_ptr<LogicalOperator> child) {
		children.push_back(move(child));
	}

	virtual ColumnBinding PushdownColumnBinding(ColumnBinding &binding) {
		return children[0]->PushdownColumnBinding(binding);
	}

	virtual idx_t EstimateCardinality() {
		// simple estimator, just take the max of the children
		idx_t max_cardinality = 0;
		for (auto &child : children) {
			max_cardinality = std::max(child->EstimateCardinality(), max_cardinality);
		}
		return max_cardinality;
	}

protected:
	//! Resolve types for this specific operator
	virtual void ResolveTypes() = 0;
};
} // namespace duckdb
