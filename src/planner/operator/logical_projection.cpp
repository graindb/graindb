#include "duckdb/planner/operator/logical_projection.hpp"

using namespace duckdb;
using namespace std;

LogicalProjection::LogicalProjection(idx_t table_index, vector<unique_ptr<Expression>> select_list)
    : LogicalOperator(LogicalOperatorType::PROJECTION, move(select_list)), table_index(table_index) {
}

vector<ColumnBinding> LogicalProjection::GetColumnBindings() {
	return GenerateColumnBindings(table_index, expressions.size());
}

ColumnBinding LogicalProjection::PushdownColumnBinding(ColumnBinding &binding) {
	auto child_binding = children[0]->PushdownColumnBinding(binding);
	if (child_binding.column_index != INVALID_INDEX) {
		auto new_ref_expr = make_unique<BoundColumnRefExpression>(TypeId::INT64, child_binding);
		expressions.push_back(move(new_ref_expr));
		return ColumnBinding(table_index, expressions.size() - 1);
	}
	return ColumnBinding(table_index, INVALID_INDEX);
}

void LogicalProjection::ResolveTypes() {
	for (auto &expr : expressions) {
		types.push_back(expr->return_type);
	}
}
