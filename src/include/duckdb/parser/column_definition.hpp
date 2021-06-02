//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/column_definition.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/common.hpp"
#include "duckdb/common/types/value.hpp"
#include "duckdb/parser/parsed_expression.hpp"

namespace duckdb {

//! A column of a table.
class ColumnDefinition {
public:
	ColumnDefinition(string name, SQLType type) : name(name), rai_oid(0), type(type) {
	}
	ColumnDefinition(string name, SQLType type, unique_ptr<ParsedExpression> default_value)
	    : name(name), rai_oid(0), type(type), default_value(move(default_value)) {
	}

	//! The name of the entry
	string name;
	//! The index of the column in the table
	idx_t oid;
	//! The index of referenced rai-id-column in the table
	idx_t rai_oid;
	//! The type of the column
	SQLType type;
	//! The default value of the column (if any)
	unique_ptr<ParsedExpression> default_value;

public:
	ColumnDefinition Copy();

	void Serialize(Serializer &serializer);
	static ColumnDefinition Deserialize(Deserializer &source);
};

} // namespace duckdb
