#include "benchmark_runner.hpp"
#include "compare_result.hpp"
#include "duckdb_benchmark_macro.hpp"
#include "ldbc.hpp"

using namespace duckdb;
using namespace std;

#define SF 30

#define LDBC_QUERY_BODY(QNR, PARAM, ENABLE_RAIS, JO_NAME)                                                              \
	virtual void Load(DuckDBBenchmarkState *state) {                                                                   \
		ldbc::dbgen_micro(state->conn, SF, 8, ENABLE_RAIS);                                                            \
	}                                                                                                                  \
	virtual string GetQuery() {                                                                                        \
		return ldbc::get_micro_query(QNR, PARAM);                                                                      \
	}                                                                                                                  \
	virtual string GetJO() {                                                                                           \
		return ldbc::get_optimized_jo(JO_NAME);                                                                        \
	}                                                                                                                  \
	virtual string VerifyResult(QueryResult *result) {                                                                 \
		if (!result->success) {                                                                                        \
			return result->error;                                                                                      \
		}                                                                                                              \
		return "";                                                                                                     \
	}                                                                                                                  \
	virtual string BenchmarkInfo() {                                                                                   \
		return StringUtil::Format("LDBC Light Q%d: %s", QNR, ldbc::get_light_query(QNR, SF).c_str());                  \
	}

DUCKDB_BENCHMARK(LDBC_MICRO08_Q001, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 20, false, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q001)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q001A, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 20, true, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q001A)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q002, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 40, false, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q002)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q002A, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 40, true, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q002A)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q003, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 80, false, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q003)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q003A, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 80, true, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q003A)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q004, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 150, false, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q004)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q004A, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 150, true, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q004A)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q005, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 270, false, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q005)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q005A, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 270, true, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q005A)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q006, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 640, false, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q006)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q006A, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 640, true, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q006A)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q007, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 2000, false, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q007)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q007A, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 2000, true, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q007A)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q008, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 3800, false, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q008)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q008A, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 3800, true, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q008A)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q009, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 9300, false, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q009)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q009A, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 9300, true, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q009A)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q010, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 18700, false, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q010)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q010A, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 18700, true, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q010A)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q011, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 37600, false, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q011)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q011A, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 37600, true, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q011A)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q012, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 55700, false, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q012)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q012A, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 55700, true, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q012A)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q013, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 74200, false, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q013)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q013A, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 74200, true, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q013A)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q014, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 92700, false, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q014)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q014A, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 92700, true, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q014A)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q015, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 110800, false, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q015)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q015A, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 110800, true, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q015A)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q016, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 129100, false, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q016)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q016A, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 129100, true, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q016A)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q017, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 147300, false, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q017)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q017A, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 147300, true, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q017A)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q018, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 165500, false, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q018)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q018A, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 165500, true, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q018A)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q019, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 184000, false, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q019)

DUCKDB_BENCHMARK(LDBC_MICRO08_Q019A, "[ldbc_micro_08]")
LDBC_QUERY_BODY(8, 184000, true, "MICRO08")
FINISH_BENCHMARK(LDBC_MICRO08_Q019A)
