#include "duckdb/execution/operator/schema/physical_create_rai.hpp"
#include "duckdb/execution/physical_plan_generator.hpp"

#include <duckdb/planner/operator/logical_create_rai.hpp>

using namespace duckdb;
using namespace std;

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalCreateRAI &op) {
	assert(op.children.size() == 1);
	dependencies.insert(&op.table);

	auto plan = CreatePlan(*op.children[0]);
	auto create = make_unique<PhysicalCreateRAI>(op, op.name, op.table, op.rai_direction, op.column_ids,
	                                             op.referenced_tables, op.referenced_columns);
	create->children.push_back(move(plan));
	return create;
}
