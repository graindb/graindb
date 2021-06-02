#include "duckdb/catalog/catalog_entry/table_catalog_entry.hpp"
#include "duckdb/execution/operator/join/physical_cross_product.hpp"
#include "duckdb/execution/operator/join/physical_hash_join.hpp"
#include "duckdb/execution/operator/join/physical_merge_sip_join.hpp"
#include "duckdb/execution/operator/join/physical_nested_loop_join.hpp"
#include "duckdb/execution/operator/join/physical_piecewise_merge_join.hpp"
#include "duckdb/execution/operator/join/physical_sip_join.hpp"
#include "duckdb/execution/physical_plan_generator.hpp"
#include "duckdb/planner/operator/logical_comparison_join.hpp"

using namespace duckdb;
using namespace std;

static inline unique_ptr<PhysicalOperator> CreateDefaultJoinPlan(ClientContext &context, LogicalComparisonJoin &op,
                                                                 unique_ptr<PhysicalOperator> left,
                                                                 unique_ptr<PhysicalOperator> right) {
	unique_ptr<PhysicalOperator> plan;
	bool has_equality = false;
	bool has_inequality = false;
#ifndef NDEBUG
	bool has_null_equal_conditions = false;
#endif
	for (auto &cond : op.conditions) {
		if (cond.comparison == ExpressionType::COMPARE_EQUAL) {
			has_equality = true;
		}
		if (cond.comparison == ExpressionType::COMPARE_NOTEQUAL) {
			has_inequality = true;
		}
		if (cond.null_values_are_equal) {
#ifndef NDEBUG
			has_null_equal_conditions = true;
#endif
			assert(cond.comparison == ExpressionType::COMPARE_EQUAL);
		}
	}
	if (has_equality) {
		// equality join: use hash join
		plan = make_unique<PhysicalHashJoin>(context, op, move(left), move(right), move(op.conditions), op.join_type,
		                                     op.left_projection_map, op.right_projection_map);
	} else {
		assert(!has_null_equal_conditions); // don't support this for anything but hash joins for now
		if (op.conditions.size() == 1 && (op.join_type == JoinType::MARK || op.join_type == JoinType::INNER) &&
		    !has_inequality) {
			// range join: use piecewise merge join
			plan =
			    make_unique<PhysicalPiecewiseMergeJoin>(op, move(left), move(right), move(op.conditions), op.join_type);
		} else {
			// inequality join: use nested loop
			plan = make_unique<PhysicalNestedLoopJoin>(op, move(left), move(right), move(op.conditions), op.join_type);
		}
	}

	return plan;
}

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalComparisonJoin &op) {
	// now visit the children
	assert((op.op_mark == OpMark::MERGED_SIP_JOIN || op.op_mark == OpMark::ADAPTIVE_MERGE_SIP_JOIN)
	           ? op.children.size() == 3
	           : op.children.size() == 2);

	auto left = CreatePlan(*op.children[0]);
	auto right = CreatePlan(*op.children[1]);
	assert(left && right);

	if (op.conditions.empty()) {
		// no conditions: insert a cross product
		return make_unique<PhysicalCrossProduct>(op, move(left), move(right));
	}

	unique_ptr<PhysicalOperator> plan;
	switch (op.op_mark) {
	case OpMark::HASH_JOIN: {
		// hash join
		plan = make_unique<PhysicalHashJoin>(context, op, move(left), move(right), move(op.conditions), op.join_type,
		                                     op.left_projection_map, op.right_projection_map);
		break;
	}
	case OpMark::SIP_JOIN: {
		plan = make_unique<PhysicalSIPJoin>(context, op, move(left), move(right), move(op.conditions), op.join_type,
		                                    op.left_projection_map, op.right_projection_map);
		break;
	}
	case OpMark::MERGED_SIP_JOIN: {
		plan =
		    make_unique<PhysicalMergeSIPJoin>(context, op, move(left), move(right), move(op.conditions), op.join_type,
		                                      op.left_projection_map, op.right_projection_map, op.merge_projection_map);
		break;
	}
	default: {
		// fall back to default rule-based join algorithm selection
		plan = CreateDefaultJoinPlan(context, op, move(left), move(right));
		break;
	}
	}

	return plan;
}
