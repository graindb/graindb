#include "catch.hpp"
#include "duckdb/common/string_util.hpp"
#include "imdb.hpp"
#include "test_helpers.hpp"

#include <chrono>
#include <iostream>

using namespace duckdb;
using namespace std;

TEST_CASE("Test JOB small without rai", "[job]") {
	unique_ptr<QueryResult> result;

	// generate the TPC-H data for SF 0.1
	DuckDB db(nullptr);
	Connection con(db);
	imdb::dbgen_small(con, false);

	// test all the basic queries
	for (idx_t i = 1; i < 113; i++) {
		REQUIRE_NO_FAIL(con.Query(imdb::get_113_query(i)));
	}
}

TEST_CASE("Test JOB small with rai", "[job]") {
	unique_ptr<QueryResult> result;

	// generate the TPC-H data for SF 0.1
	DuckDB db(nullptr);
	Connection con(db);
	imdb::dbgen_small(con, true);

	// test all the basic queries
	for (idx_t i = 1; i < 113; i++) {
		REQUIRE_NO_FAIL(con.Query(imdb::get_113_query(i)));
	}
}
