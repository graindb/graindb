//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/planner/column_binding.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/catalog/catalog_entry/table_catalog_entry.hpp"
#include "duckdb/common/common.hpp"
#include "duckdb/storage/rai.hpp"

using std::hash;

namespace duckdb {

struct ColumnBinding {
	idx_t table_index;
	idx_t column_index;
	idx_t column_ordinal;
	TableCatalogEntry *table;

	ColumnBinding()
	    : table_index(INVALID_INDEX), column_index(INVALID_INDEX), column_ordinal(INVALID_INDEX), table(nullptr) {
	}
	ColumnBinding(idx_t table, idx_t column)
	    : table_index(table), column_index(column), column_ordinal(column), table(nullptr) {
	}
	ColumnBinding(idx_t table_index, idx_t column_index, idx_t column_ordinal)
	    : table_index(table_index), column_index(column_index), column_ordinal(column_ordinal), table(nullptr) {
	}
	ColumnBinding(idx_t table_index, idx_t column_index, idx_t column_ordinal, TableCatalogEntry *table)
	    : table_index(table_index), column_index(column_index), column_ordinal(column_ordinal), table(table) {
	}

	bool operator==(const ColumnBinding &rhs) const {
		return table_index == rhs.table_index && column_index == rhs.column_index;
	}

	bool Match(ColumnBinding binding) const {
		return table->oid == binding.table->oid && column_ordinal == binding.column_ordinal;
	}

	bool Match(TableCatalogEntry *table_, idx_t column_ordinal_) const {
		return table_->name == table->name && column_ordinal_ == column_ordinal;
	}
};

struct ColumnBindingHasher {
	size_t operator()(const ColumnBinding &binding) const {
		return ((hash<idx_t>()(binding.table_index) ^ hash<idx_t>()(binding.column_index) << 1) >> 1);
	}
};

} // namespace duckdb
