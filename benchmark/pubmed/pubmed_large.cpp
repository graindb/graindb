#include "benchmark_runner.hpp"
#include "compare_result.hpp"
#include "duckdb_benchmark_macro.hpp"
#include "pubmed.hpp"

using namespace duckdb;
using namespace std;

#define ALL_RAIS                                                                                                       \
	"CREATE UNDIRECTED RAI da_rai ON da (FROM doc_id REFERENCES documents.doc_id, TO au_id REFERENCES authors.au_id);" \
	"CREATE UNDIRECTED RAI dt_rai ON dt (FROM doc_id REFERENCES documents.doc_id, TO t_id REFERENCES terms.t_id);"

#define PUBMED_QUERY_BODY(QNR, PARAM1, PARAM2, RAI_STMT, JO_NAME)                                                      \
	virtual void Load(DuckDBBenchmarkState *state) {                                                                   \
		pubmed::dbgen_large(state->conn, RAI_STMT);                                                                    \
	}                                                                                                                  \
	virtual string GetQuery() {                                                                                        \
		return pubmed::get_query(QNR, PARAM1, PARAM2);                                                                 \
	}                                                                                                                  \
	virtual string GetJO() {                                                                                           \
		return pubmed::get_jo(JO_NAME);                                                                                \
	}                                                                                                                  \
	virtual string VerifyResult(QueryResult *result) {                                                                 \
		if (!result->success) {                                                                                        \
			return result->error;                                                                                      \
		}                                                                                                              \
		return "";                                                                                                     \
	}                                                                                                                  \
	virtual string BenchmarkInfo() {                                                                                   \
		return StringUtil::Format("PUBMED Q%d: %s", QNR, pubmed::get_query(QNR, PARAM1, PARAM2).c_str());              \
	}

// SD
DUCKDB_BENCHMARK(PUBMED_SD_Q001, "[pubmed_large]")
PUBMED_QUERY_BODY(1, 100, 0, "", "SD01");
FINISH_BENCHMARK(PUBMED_SD_Q001);

DUCKDB_BENCHMARK(PUBMED_SD_Q001A, "[pubmed_large]")
PUBMED_QUERY_BODY(1, 100, 0, ALL_RAIS, "SD01");
FINISH_BENCHMARK(PUBMED_SD_Q001A);

DUCKDB_BENCHMARK(PUBMED_SD_Q002, "[pubmed_large]")
PUBMED_QUERY_BODY(1, 1000, 0, "", "SD02");
FINISH_BENCHMARK(PUBMED_SD_Q002);

DUCKDB_BENCHMARK(PUBMED_SD_Q002A, "[pubmed_large]")
PUBMED_QUERY_BODY(1, 1000, 0, ALL_RAIS, "SD02");
FINISH_BENCHMARK(PUBMED_SD_Q002A);

// FSD
DUCKDB_BENCHMARK(PUBMED_FSD_Q001, "[pubmed_large]")
PUBMED_QUERY_BODY(2, 100, 0, "", "FSD01");
FINISH_BENCHMARK(PUBMED_FSD_Q001);

DUCKDB_BENCHMARK(PUBMED_FSD_Q001A, "[pubmed_large]")
PUBMED_QUERY_BODY(2, 100, 0, ALL_RAIS, "FSD01");
FINISH_BENCHMARK(PUBMED_FSD_Q001A);

DUCKDB_BENCHMARK(PUBMED_FSD_Q002, "[pubmed_large]")
PUBMED_QUERY_BODY(2, 1000, 0, "", "FSD01");
FINISH_BENCHMARK(PUBMED_FSD_Q002);

DUCKDB_BENCHMARK(PUBMED_FSD_Q002A, "[pubmed_large]")
PUBMED_QUERY_BODY(2, 1000, 0, ALL_RAIS, "FSD01");
FINISH_BENCHMARK(PUBMED_FSD_Q002A);

// AS
DUCKDB_BENCHMARK(PUBMED_AS_Q001, "[pubmed_large]")
PUBMED_QUERY_BODY(3, 100, 0, "", "AS01");
FINISH_BENCHMARK(PUBMED_AS_Q001);

DUCKDB_BENCHMARK(PUBMED_AS_Q001A, "[pubmed_large]")
PUBMED_QUERY_BODY(3, 100, 0, ALL_RAIS, "AS01");
FINISH_BENCHMARK(PUBMED_AS_Q001A);

DUCKDB_BENCHMARK(PUBMED_AS_Q002, "[pubmed_large]")
PUBMED_QUERY_BODY(3, 1000, 0, "", "AS01");
FINISH_BENCHMARK(PUBMED_AS_Q002);

DUCKDB_BENCHMARK(PUBMED_AS_Q002A, "[pubmed_large]")
PUBMED_QUERY_BODY(3, 1000, 0, ALL_RAIS, "AS01");
FINISH_BENCHMARK(PUBMED_AS_Q002A);

// AD
DUCKDB_BENCHMARK(PUBMED_AD_Q001, "[pubmed_large]")
PUBMED_QUERY_BODY(4, 5232, 413, "", "AD01");
FINISH_BENCHMARK(PUBMED_AD_Q001);

DUCKDB_BENCHMARK(PUBMED_AD_Q001A, "[pubmed_large]")
PUBMED_QUERY_BODY(4, 5232, 413, ALL_RAIS, "AD01");
FINISH_BENCHMARK(PUBMED_AD_Q001A);

DUCKDB_BENCHMARK(PUBMED_AD_Q002, "[pubmed_large]")
PUBMED_QUERY_BODY(4, 100, 1000, "", "AD02");
FINISH_BENCHMARK(PUBMED_AD_Q002);

DUCKDB_BENCHMARK(PUBMED_AD_Q002A, "[pubmed_large]")
PUBMED_QUERY_BODY(4, 100, 1000, ALL_RAIS, "AD02");
FINISH_BENCHMARK(PUBMED_AD_Q002A);
