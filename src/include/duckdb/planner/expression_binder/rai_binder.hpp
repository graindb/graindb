//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/planner/expression_binder/rai_binder.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/planner/expression_binder.hpp"

namespace duckdb {
class BoundColumnRefExpression;

class RAIBinder : public ExpressionBinder {
public:
	RAIBinder(Binder &binder, ClientContext &context);

protected:
	BindResult BindExpression(ParsedExpression &expr, idx_t depth, bool root_expression = false) override;

	string UnsupportedAggregateMessage() override;
};
} // namespace duckdb
