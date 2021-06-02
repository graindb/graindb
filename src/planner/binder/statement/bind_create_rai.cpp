#include "duckdb/common/enums/join_type.hpp"
#include "duckdb/parser/parsed_data/create_rai_info.hpp"
#include "duckdb/planner/binder.hpp"
#include "duckdb/planner/expression/bound_comparison_expression.hpp"
#include "duckdb/planner/expression_binder/rai_binder.hpp"
#include "duckdb/planner/operator/logical_comparison_join.hpp"
#include "duckdb/planner/operator/logical_create_rai.hpp"
#include "duckdb/planner/operator/logical_cross_product.hpp"
#include "duckdb/planner/operator/logical_get.hpp"
#include "duckdb/planner/operator/logical_order.hpp"
#include "duckdb/planner/operator/logical_projection.hpp"
#include "duckdb/planner/parsed_data/bound_create_rai_info.hpp"
#include "duckdb/planner/tableref/bound_basetableref.hpp"

namespace duckdb {

unique_ptr<BoundCreateRAIInfo> Binder::BindCreateRAIInfo(unique_ptr<CreateInfo> info) {
	auto &base = (CreateRAIInfo &)*info;
	auto result = make_unique<BoundCreateRAIInfo>(move(info));

	result->name = base.name;
	result->table = Bind(*base.table);
	result->rai_direction = base.direction;
	for (auto &ref : base.referenced_tables) {
		result->referenced_tables.push_back(Bind(*ref));
	}

	RAIBinder binder(*this, context);
	for (auto &expr : base.columns) {
		result->columns.push_back(binder.Bind(expr));
	}
	for (auto &expr : base.references) {
		result->references.push_back(binder.Bind(expr));
	}

	auto plan = CreatePlan(*result);
	result->plan = move(plan);

	return result;
}

static unique_ptr<LogicalOperator> CreatePlanForPKFKRAI(BoundCreateRAIInfo &bound_info) {
	vector<column_t> referenced_column_ids;
	vector<column_t> base_column_ids;
	vector<TableCatalogEntry *> referenced_tables;

	auto from_tbl_ref = reinterpret_cast<BoundBaseTableRef *>(bound_info.referenced_tables[0].get());
	auto base_tbl_ref = reinterpret_cast<BoundBaseTableRef *>(bound_info.table.get());
	auto from_get = reinterpret_cast<LogicalGet *>(from_tbl_ref->get.get());
	auto base_get = reinterpret_cast<LogicalGet *>(base_tbl_ref->get.get());
	from_get->column_ids.push_back((column_t)-1);
	base_get->column_ids.push_back((column_t)-1);

	string col_name = bound_info.references[0]->GetName();
	auto entry = from_get->table->name_map.find(col_name);
	if (entry != from_get->table->name_map.end()) {
		referenced_column_ids.push_back(entry->second);
	} else {
		throw Exception("Column " + col_name + " in rai not found");
	}
	col_name = bound_info.references[1]->GetName();
	for (auto &col : bound_info.columns) {
		entry = base_get->table->name_map.find(col->GetName());
		if (entry != base_get->table->name_map.end()) {
			base_column_ids.push_back(entry->second);
		} else {
			throw Exception("Column " + col->GetName() + " in rai not found");
		}
	}

	referenced_tables.push_back(from_get->table);
	referenced_tables.push_back(base_get->table);

	auto join = make_unique<LogicalComparisonJoin>(JoinType::LEFT);
	join->AddChild(move(base_tbl_ref->get));
	join->AddChild(move(from_tbl_ref->get));
	JoinCondition join_condition;
	join_condition.comparison = ExpressionType::COMPARE_EQUAL;
	join_condition.left = move(bound_info.columns[0]);
	join_condition.right = move(bound_info.references[0]);
	join->conditions.push_back(move(join_condition));

	ColumnBinding proj_0(from_tbl_ref->table_index, 1);
	ColumnBinding proj_1(base_tbl_ref->table_index, 1);
	vector<unique_ptr<Expression>> selection_list;
	selection_list.push_back(make_unique<BoundColumnRefExpression>(TypeId::INT64, proj_0));
	selection_list.push_back(make_unique<BoundColumnRefExpression>(TypeId::INT64, proj_1));
	auto projection = make_unique<LogicalProjection>(0, move(selection_list));
	projection->AddChild(move(join));

	ColumnBinding order_0(0, 0);
	ColumnBinding order_1(0, 1);
	BoundOrderByNode order_0_node(OrderType::ASCENDING, make_unique<BoundColumnRefExpression>(TypeId::INT64, order_0));
	BoundOrderByNode order_1_node(OrderType::ASCENDING, make_unique<BoundColumnRefExpression>(TypeId::INT64, order_1));
	vector<BoundOrderByNode> orders;
	orders.push_back(move(order_1_node));
	auto order_by = make_unique<LogicalOrder>(move(orders));
	order_by->AddChild(move(projection));

	auto create_rai = make_unique<LogicalCreateRAI>(bound_info.name, *base_get->table, bound_info.rai_direction,
	                                                base_column_ids, referenced_tables, referenced_column_ids);
	create_rai->AddChild(move(order_by));

	return create_rai;
}

static unique_ptr<LogicalOperator> CreatePlanForNonPKFPRAI(BoundCreateRAIInfo &bound_info) {
	vector<column_t> referenced_column_ids;
	vector<column_t> base_column_ids;
	vector<TableCatalogEntry *> referenced_tables;

	auto ref_from_tbl = reinterpret_cast<BoundBaseTableRef *>(bound_info.referenced_tables[0].get());
	auto ref_to_tbl = reinterpret_cast<BoundBaseTableRef *>(bound_info.referenced_tables[1].get());
	auto base_tbl = reinterpret_cast<BoundBaseTableRef *>(bound_info.table.get());
	auto ref_from_get = reinterpret_cast<LogicalGet *>(ref_from_tbl->get.get());
	auto ref_to_get = reinterpret_cast<LogicalGet *>(ref_to_tbl->get.get());
	auto base_get = reinterpret_cast<LogicalGet *>(base_tbl->get.get());
	ref_from_get->column_ids.push_back((column_t)-1);
	ref_to_get->column_ids.push_back((column_t)-1);
	base_get->column_ids.push_back((column_t)-1);

	string col_name = bound_info.references[0]->GetName();
	auto entry = ref_from_get->table->name_map.find(col_name);
	if (entry != ref_from_get->table->name_map.end()) {
		referenced_column_ids.push_back(entry->second);
	} else {
		throw Exception("Column " + col_name + " in rai not found");
	}
	col_name = bound_info.references[1]->GetName();
	entry = ref_to_get->table->name_map.find(col_name);
	if (entry != ref_to_get->table->name_map.end()) {
		referenced_column_ids.push_back(entry->second);
	} else {
		throw Exception("Column " + col_name + " in rai not found");
	}
	for (auto &col : bound_info.columns) {
		entry = base_get->table->name_map.find(col->GetName());
		if (entry != base_get->table->name_map.end()) {
			base_column_ids.push_back(entry->second);
		} else {
			throw Exception("Column " + col->GetName() + " in rai not found");
		}
	}

	referenced_tables.push_back(ref_from_get->table);
	referenced_tables.push_back(ref_to_get->table);

	auto join = make_unique<LogicalComparisonJoin>(JoinType::LEFT);
	join->AddChild(move(base_tbl->get));
	join->AddChild(move(ref_from_tbl->get));
	JoinCondition join_condition;
	join_condition.comparison = ExpressionType::COMPARE_EQUAL;
	join_condition.left = move(bound_info.columns[0]);
	join_condition.right = move(bound_info.references[0]);
	join->conditions.push_back(move(join_condition));

	auto parent_join = make_unique<LogicalComparisonJoin>(JoinType::LEFT);
	parent_join->AddChild(move(join));
	parent_join->AddChild(move(ref_to_tbl->get));
	JoinCondition parent_join_condition;
	parent_join_condition.comparison = ExpressionType::COMPARE_EQUAL;
	parent_join_condition.left = move(bound_info.columns[1]);
	parent_join_condition.right = move(bound_info.references[1]);
	parent_join->conditions.push_back(move(parent_join_condition));

	ColumnBinding proj_0(ref_from_tbl->table_index, 1);
	ColumnBinding proj_1(base_tbl->table_index, 2);
	ColumnBinding proj_2(ref_to_tbl->table_index, 1);
	vector<unique_ptr<Expression>> selection_list;
	selection_list.push_back(make_unique<BoundColumnRefExpression>(TypeId::INT64, proj_0));
	selection_list.push_back(make_unique<BoundColumnRefExpression>(TypeId::INT64, proj_1));
	selection_list.push_back(make_unique<BoundColumnRefExpression>(TypeId::INT64, proj_2));
	auto projection = make_unique<LogicalProjection>(0, move(selection_list));
	projection->AddChild(move(parent_join));

	ColumnBinding order_0(0, 0);
	ColumnBinding order_1(0, 1);
	BoundOrderByNode order_0_node(OrderType::ASCENDING, make_unique<BoundColumnRefExpression>(TypeId::INT64, order_0));
	BoundOrderByNode order_1_node(OrderType::ASCENDING, make_unique<BoundColumnRefExpression>(TypeId::INT64, order_1));
	vector<BoundOrderByNode> orders;
	orders.push_back(move(order_1_node));
	auto order_by = make_unique<LogicalOrder>(move(orders));
	order_by->AddChild(move(projection));

	auto create_rai = make_unique<LogicalCreateRAI>(bound_info.name, *base_get->table, bound_info.rai_direction,
	                                                base_column_ids, referenced_tables, referenced_column_ids);
	create_rai->AddChild(move(order_by));

	return create_rai;
}

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundCreateRAIInfo &bound_info) {
	if (bound_info.rai_direction == RAIDirection::PKFK) {
		return CreatePlanForPKFKRAI(bound_info);
	} else {
		return CreatePlanForNonPKFPRAI(bound_info);
	}
}

} // namespace duckdb
