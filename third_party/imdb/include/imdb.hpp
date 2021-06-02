#pragma once

#include "duckdb.hpp"
#include "duckdb/catalog/catalog.hpp"

namespace imdb {
//! Adds the IMDB tables to the database
void dbgen(duckdb::Connection &conn, bool enableRAIs, bool enableLoadData = true);
void dbgen_small(duckdb::Connection &conn, bool enableRAIs, bool enableLoadData = true);

//! Gets the specified IMDB JOB query join order as a string
std::string get_113_default_jo(std::string jo_name);
std::string get_113_optimized_jo(std::string jo_name);
//! Gets the specified IMDB JOB qeury cardinality as a string
std::string get_113_card(std::string card_name);
//! Gets the specified IMDB JOB Query as a string
std::string get_113_query(int query);
int get_query_group_id(int query);
//! Gets the IMDB table name
std::string get_table(int tid);
//! Gets the IMDB table ddl
std::string get_ddl(int tid);
//! Gets the answer to the specified IMDB JOB Query
std::string get_answer(int query);

} // namespace imdb
