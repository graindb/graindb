//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/common/enums/column_type.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/constants.hpp"

namespace duckdb {

enum class ColumnType : uint8_t { COMMON = 0, FROM = 1, TO = 2, LABEL = 3 };
}
