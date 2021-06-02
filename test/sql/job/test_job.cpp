#include "catch.hpp"
#include "imdb.hpp"
#include "duckdb/common/string_util.hpp"
#include "test_helpers.hpp"

#include <chrono>

using namespace duckdb;
using namespace std;

#define ALL_RAIS \
"CREATE UNDIRECTED RAI it_midx_t ON movie_info_idx (FROM info_type_id REFERENCES info_type.id, TO movie_id REFERENCES title.id);" \
"CREATE UNDIRECTED RAI cn_mc_t ON movie_companies (FROM company_id REFERENCES company_name.id, TO movie_id REFERENCES title.id);" \
"CREATE UNDIRECTED RAI k_mk_t ON movie_keyword (FROM keyword_id REFERENCES keyword.id, TO movie_id REFERENCES title.id);" \
"CREATE UNDIRECTED RAI it_mi_t ON movie_info (FROM info_type_id REFERENCES info_type.id, TO movie_id REFERENCES title.id);" \
"CREATE UNDIRECTED RAI lt_ml_t ON movie_link (FROM link_type_id REFERENCES link_type.id, TO movie_id REFERENCES title.id);" \
"CREATE UNDIRECTED RAI it_pi_n ON person_info (FROM info_type_id REFERENCES info_type.id, TO person_id REFERENCES name.id);" \
"CREATE UNDIRECTED RAI t_ci_n ON cast_info (FROM movie_id REFERENCES title.id, TO person_id REFERENCES name.id);" \
"CREATE UNDIRECTED RAI rt_ci_n ON cast_info (FROM role_id REFERENCES role_type.id, TO person_id REFERENCES name.id);" \
"CREATE UNDIRECTED RAI rt_ci_t ON cast_info (FROM role_id REFERENCES role_type.id, TO movie_id REFERENCES title.id);" \
"CREATE UNDIRECTED RAI cct_cc_t_subject ON complete_cast (FROM subject_id REFERENCES comp_cast_type.id, TO movie_id REFERENCES title.id);"\
"CREATE UNDIRECTED RAI cct_cc_t_status ON complete_cast (FROM status_id REFERENCES comp_cast_type.id, TO movie_id REFERENCES title.id);"

TEST_CASE("Test JOB small without rai", "[job]") {
	unique_ptr<QueryResult> result;

	// generate the TPC-H data for SF 0.1
	DuckDB db(nullptr);
	Connection con(db);
	imdb::dbgen(con);

	// test all the basic queries
	for (idx_t i = 0; i < 113; i++) {
		REQUIRE_NO_FAIL(con.Query(imdb::get_113_query(i)));
	}
}

TEST_CASE("Test JOB small with rai", "[job]") {
	unique_ptr<QueryResult> result;

	// generate the TPC-H data for SF 0.1
	DuckDB db(nullptr);
	Connection con(db);
	imdb::dbgen(con, ALL_RAIS);

	// test all the basic queries
	for (idx_t i = 0; i < 113; i++) {
		REQUIRE_NO_FAIL(con.Query(imdb::get_113_query(i)));
	}
}
