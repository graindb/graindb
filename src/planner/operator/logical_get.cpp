#include "duckdb/planner/operator/logical_get.hpp"

#include "duckdb/catalog/catalog_entry/table_catalog_entry.hpp"

using namespace duckdb;
using namespace std;

LogicalGet::LogicalGet(idx_t table_index)
    : LogicalOperator(LogicalOperatorType::GET), table(nullptr), table_index(table_index) {
}
LogicalGet::LogicalGet(TableCatalogEntry *table, idx_t table_index)
    : LogicalOperator(LogicalOperatorType::GET), table(table), table_index(table_index) {
}
LogicalGet::LogicalGet(TableCatalogEntry *table, idx_t table_index, vector<column_t> column_ids)
    : LogicalOperator(LogicalOperatorType::GET), table(table), table_index(table_index), column_ids(column_ids) {
}

string LogicalGet::ParamsToString() const {
	if (!table) {
		return "";
	}
	string result = "[" + table->name + "<" + to_string(table_index) + ">" + " (";
	if (column_ids.size() > 0) {
		result += StringUtil::Join(column_ids, column_ids.size(), ", ", [&](const idx_t cid) {
			if (cid == COLUMN_IDENTIFIER_ROW_ID) {
				return "rowid";
			}
			return table->columns[cid].name.c_str();
		});
	}
	return result + ")]";
}

vector<ColumnBinding> LogicalGet::GetColumnBindings() {
	if (!table) {
		return {ColumnBinding(INVALID_INDEX, 0)};
	}
	if (column_ids.size() == 0) {
		return {ColumnBinding(table_index, 0)};
	}
	vector<ColumnBinding> result;
	for (idx_t i = 0; i < column_ids.size(); i++) {
		result.push_back(ColumnBinding(table_index, i));
	}
	return result;
}

ColumnBinding LogicalGet::PushdownColumnBinding(ColumnBinding &binding) {
	column_t referenced_rai = binding.column_index == COLUMN_IDENTIFIER_ROW_ID
	                              ? COLUMN_IDENTIFIER_ROW_ID
	                              : table->columns[binding.column_index].rai_oid;
	auto entry = find(column_ids.begin(), column_ids.end(), referenced_rai);
	if (entry != column_ids.end()) {
		auto column_idx = distance(column_ids.begin(), entry);
		return ColumnBinding(table_index, column_idx, referenced_rai, table);
	} else {
		column_ids.push_back(referenced_rai);
		return ColumnBinding(table_index, column_ids.size() - 1, referenced_rai, table);
	}
}

void LogicalGet::ResolveTypes() {
	if (column_ids.size() == 0) {
		column_ids.push_back(COLUMN_IDENTIFIER_ROW_ID);
	}
	types = table->GetTypes(column_ids);
}

idx_t LogicalGet::EstimateCardinality() {
	if (table) {
		return table->storage->info->cardinality;
	} else {
		return 1;
	}
}
