#pragma once

#include "duckdb.hpp"

namespace pubmed {
void dbgen_small(duckdb::Connection &conn, std::string rai_stmt = "");
void dbgen_large(duckdb::Connection &conn, std::string rai_stmt = "");

std::string get_jo(std::string name);

std::string get_sd(int64_t doc_id);
std::string get_fsd(int64_t doc_id);
std::string get_as(int64_t author_id);
std::string get_ad(int64_t term1_id, int64_t term2id);
std::string get_query(int id, int64_t param1, int64_t param2);
}
