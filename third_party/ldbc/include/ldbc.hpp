#pragma once

#include "duckdb.hpp"
#include "duckdb/catalog/catalog.hpp"

using namespace duckdb;

namespace ldbc {

void dbgen(duckdb::Connection &conn, int sf, bool enableRAIs, bool enableLoading = true);
void dbgen_micro(duckdb::Connection &conn, int sf, int qgid, bool enableRAIs, bool enableLoading = true);

string get_table_name(int num);
string get_ddl(int num);
//! Gets the specified LDBC SNB Query number as a string
std::string get_query(int query, int sf);
std::string get_light_query(int query, int sf);
std::string get_micro_query(int query, int param);
//! Gets the specified LDBC SNB Query Join Order
std::string get_default_jo(std::string jo_name);
std::string get_optimized_jo(std::string jo_name);

} // namespace ldbc
