//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/common/enums/physical_operator_type.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/constants.hpp"

namespace duckdb {

//===--------------------------------------------------------------------===//
// Physical Operator Types
//===--------------------------------------------------------------------===//
enum class PhysicalOperatorType : uint8_t {
	INVALID,
	LEAF,
	ORDER_BY,
	LIMIT,
	TOP_N,
	AGGREGATE,
	WINDOW,
	UNNEST,
	DISTINCT,
	SIMPLE_AGGREGATE,
	HASH_GROUP_BY,
	SORT_GROUP_BY,
	FILTER,
	PROJECTION,
	COPY_FROM_FILE,
	COPY_TO_FILE,
	TABLE_FUNCTION,
	// -----------------------------
	// Scans
	// -----------------------------
	DUMMY_SCAN,
	SEQ_SCAN,
	INDEX_SCAN,
	LOOKUP,
	CHUNK_SCAN,
	DELIM_SCAN,
	EXTERNAL_FILE_SCAN,
	QUERY_DERIVED_SCAN,
	EXPRESSION_SCAN,
	// -----------------------------
	// Joins
	// -----------------------------
	BLOCKWISE_NL_JOIN,
	NESTED_LOOP_JOIN,
	HASH_JOIN,
	CROSS_PRODUCT,
	PIECEWISE_MERGE_JOIN,
	DELIM_JOIN,
	ADJACENCY_JOIN,
	SIP_JOIN,
	ADAPTIVE_SIP_JOIN,
	RAI_JOIN,
	MERGE_RAI_JOIN,
	MERGE_SIP_JOIN,
	ADAPTIVE_MERGE_SIP_JOIN,

	// -----------------------------
	// SetOps
	// -----------------------------
	UNION,
	RECURSIVE_CTE,

	// -----------------------------
	// Updates
	// -----------------------------
	INSERT,
	INSERT_SELECT,
	DELETE,
	UPDATE,
	EXPORT_EXTERNAL_FILE,

	// -----------------------------
	// Schema
	// -----------------------------
	CREATE,
	CREATE_INDEX,
	CREATE_RAI,
	ALTER,
	CREATE_SEQUENCE,
	CREATE_VIEW,
	CREATE_SCHEMA,
	DROP,
	PRAGMA,
	TRANSACTION,

	// -----------------------------
	// Helpers
	// -----------------------------
	EXPLAIN,
	EMPTY_RESULT,
	EXECUTE,
	PREPARE,
	VACUUM
};

string PhysicalOperatorToString(PhysicalOperatorType type);

} // namespace duckdb
