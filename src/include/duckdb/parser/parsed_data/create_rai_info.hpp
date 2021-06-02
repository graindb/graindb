//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/parsed_data/create_edge_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/enums/rai_direction.hpp"
#include "duckdb/parser/parsed_data/create_info.hpp"
#include "duckdb/parser/tableref/basetableref.hpp"

namespace duckdb {

struct CreateRAIInfo : public CreateInfo {
	CreateRAIInfo() : CreateInfo(CatalogType::RAI), name(""), table(nullptr), direction(RAIDirection::DIRECTED) {
	}

	string name;
	unique_ptr<TableRef> table;
	RAIDirection direction;
	vector<unique_ptr<TableRef>> referenced_tables;
	vector<unique_ptr<ParsedExpression>> columns;
	vector<unique_ptr<ParsedExpression>> references;
};
} // namespace duckdb
