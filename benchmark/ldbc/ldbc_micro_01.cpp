#include "benchmark_runner.hpp"
#include "compare_result.hpp"
#include "duckdb_benchmark_macro.hpp"
#include "ldbc.hpp"

using namespace duckdb;
using namespace std;

#define SF 30

#define LDBC_QUERY_BODY(QNR, PARAM, ENABLE_RAIS, JO_NAME)                                                              \
	virtual void Load(DuckDBBenchmarkState *state) {                                                                   \
		ldbc::dbgen_micro(state->conn, SF, 1, ENABLE_RAIS);                                                            \
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

DUCKDB_BENCHMARK(LDBC_MICRO01_Q001, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 2000, false, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q001)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q001A, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 2000, true, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q001A)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q002, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 3800, false, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q002)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q002A, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 3800, true, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q002A)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q003, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 9300, false, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q003)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q003A, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 9300, true, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q003A)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q004, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 18700, false, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q004)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q004A, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 18700, true, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q004A)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q005, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 37600, false, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q005)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q005A, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 37600, true, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q005A)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q006, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 55700, false, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q006)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q006A, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 55700, true, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q006A)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q007, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 74200, false, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q007)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q007A, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 74200, true, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q007A)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q008, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 92700, false, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q008)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q008A, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 92700, true, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q008A)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q009, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 110800, false, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q009)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q009A, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 110800, true, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q009A)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q010, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 129100, false, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q010)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q010A, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 129100, true, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q010A)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q011, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 147300, false, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q011)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q011A, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 147300, true, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q011A)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q012, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 165500, false, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q012)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q012A, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 165500, true, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q012A)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q013, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 184000, false, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q013)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q013A, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 184000, true, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q013A)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q014, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 20, false, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q014)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q014A, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 20, true, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q014A)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q015, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 40, false, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q015)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q015A, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 40, true, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q015A)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q016, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 80, false, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q016)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q016A, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 80, true, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q016A)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q017, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 150, false, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q017)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q017A, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 150, true, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q017A)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q018, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 270, false, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q018)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q018A, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 270, true, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q018A)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q019, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 640, false, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q019)

DUCKDB_BENCHMARK(LDBC_MICRO01_Q019A, "[ldbc_micro_01]")
LDBC_QUERY_BODY(1, 640, true, "MICRO01")
FINISH_BENCHMARK(LDBC_MICRO01_Q019A)
