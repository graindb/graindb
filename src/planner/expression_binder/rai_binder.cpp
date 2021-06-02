#include "duckdb/planner/expression_binder/rai_binder.hpp"

using namespace duckdb;
using namespace std;

RAIBinder::RAIBinder(Binder &binder, ClientContext &context) : ExpressionBinder(binder, context) {
}

BindResult RAIBinder::BindExpression(ParsedExpression &expr, idx_t depth, bool root_expression) {
	switch (expr.expression_class) {
	case ExpressionClass::COLUMN_REF:
		return ExpressionBinder::BindExpression(expr, 0);
	default:
		return BindResult("expression not allowed in edge expressions");
	}
}

string RAIBinder::UnsupportedAggregateMessage() {
	return "aggregate functions are not allowed in edge expressions";
}
