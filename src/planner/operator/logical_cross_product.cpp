#include "duckdb/planner/operator/logical_cross_product.hpp"

using namespace duckdb;
using namespace std;

LogicalCrossProduct::LogicalCrossProduct() : LogicalOperator(LogicalOperatorType::CROSS_PRODUCT) {
}

vector<ColumnBinding> LogicalCrossProduct::GetColumnBindings() {
	auto left_bindings = children[0]->GetColumnBindings();
	auto right_bindings = children[1]->GetColumnBindings();
	left_bindings.insert(left_bindings.end(), right_bindings.begin(), right_bindings.end());
	return left_bindings;
}

ColumnBinding LogicalCrossProduct::PushdownColumnBinding(ColumnBinding &binding) {
	unordered_set<idx_t> left_tables, right_tables;
	auto left_bindings = children[0]->GetColumnBindings();
	for (auto &lb : left_bindings) {
		left_tables.insert(lb.table_index);
	}
	if (left_tables.find(binding.table_index) != left_tables.end()) {
		auto child_binding = children[0]->PushdownColumnBinding(binding);
		if (child_binding.column_index != INVALID_INDEX) {
			return child_binding;
		}
	}
	auto right_bindings = children[1]->GetColumnBindings();
	for (auto &rb : right_bindings) {
		right_tables.insert(rb.table_index);
	}
	if (right_tables.find(binding.table_index) != right_tables.end()) {
		auto child_binding = children[1]->PushdownColumnBinding(binding);
		if (child_binding.column_index != INVALID_INDEX) {
			return child_binding;
		}
	}

	return ColumnBinding(binding.table_index, INVALID_INDEX);
}

void LogicalCrossProduct::ResolveTypes() {
	types.insert(types.end(), children[0]->types.begin(), children[0]->types.end());
	types.insert(types.end(), children[1]->types.begin(), children[1]->types.end());
}
