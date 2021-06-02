//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/planner/operator/logical_create_rai.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/catalog/catalog_entry/table_catalog_entry.hpp"
#include "duckdb/parser/parsed_data/create_rai_info.hpp"
#include "duckdb/planner/logical_operator.hpp"

namespace duckdb {

class LogicalCreateRAI : public LogicalOperator {
public:
	LogicalCreateRAI(string name, TableCatalogEntry &table, RAIDirection rai_direction, vector<column_t> column_ids,
	                 vector<TableCatalogEntry *> referenced_tables, vector<column_t> referenced_columns)
	    : LogicalOperator(LogicalOperatorType::CREATE_RAI), name(name), table(table), rai_direction(rai_direction),
	      column_ids(column_ids), referenced_tables(referenced_tables), referenced_columns(referenced_columns) {
	}

	string name;
	TableCatalogEntry &table;
	RAIDirection rai_direction;
	vector<column_t> column_ids;
	vector<TableCatalogEntry *> referenced_tables;
	vector<column_t> referenced_columns;

protected:
	void ResolveTypes() override {
		types.push_back(TypeId::INT64);
	}
};
} // namespace duckdb
