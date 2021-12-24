#pragma once

#include "duckdb.hpp"
#include "duckdb/catalog/catalog.hpp"

namespace imdb {
//! Adds the IMDB tables to the database
void dbgen(duckdb::Connection &conn, bool enableRAIs, bool enableLoadData = true);

//! Gets the specified IMDB JOB query join order as a string
std::string get_113_default_jo(const std::string& jo_name);
std::string get_113_optimized_jo(const std::string& jo_name);
//! Gets the specified IMDB JOB Query as a string
std::string get_113_query(int query);
//! Gets the IMDB table ddl
std::string get_ddl(int tid);

} // namespace imdb
