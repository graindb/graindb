#pragma once

#include "duckdb/common/constants.hpp"

namespace duckdb {

enum class RAIDirection : uint8_t { DIRECTED, UNDIRECTED, SELF, PKFK };
}
