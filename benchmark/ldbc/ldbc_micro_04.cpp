#include "benchmark_runner.hpp"
#include "compare_result.hpp"
#include "duckdb_benchmark_macro.hpp"
#include "ldbc.hpp"

using namespace duckdb;
using namespace std;

#define SF 30

#define LDBC_QUERY_BODY(QNR, PARAM, ENABLE_RAIS, JO_NAME)                                                              \
	virtual void Load(DuckDBBenchmarkState *state) {                                                                   \
		ldbc::dbgen_micro(state->conn, SF, 4, ENABLE_RAIS);                                                            \
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

DUCKDB_BENCHMARK(LDBC_MICRO04_Q001, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 20, false, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q001)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q001A, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 20, true, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q001A)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q002, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 40, false, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q002)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q002A, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 40, true, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q002A)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q003, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 80, false, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q003)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q003A, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 80, true, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q003A)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q004, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 150, false, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q004)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q004A, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 150, true, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q004A)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q005, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 270, false, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q005)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q005A, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 270, true, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q005A)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q006, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 640, false, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q006)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q006A, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 640, true, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q006A)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q007, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 2000, false, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q007)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q007A, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 2000, true, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q007A)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q008, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 3800, false, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q008)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q008A, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 3800, true, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q008A)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q009, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 9300, false, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q009)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q009A, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 9300, true, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q009A)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q010, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 18700, false, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q010)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q010A, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 18700, true, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q010A)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q011, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 37600, false, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q011)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q011A, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 37600, true, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q011A)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q012, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 55700, false, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q012)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q012A, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 55700, true, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q012A)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q013, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 74200, false, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q013)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q013A, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 74200, true, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q013A)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q014, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 92700, false, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q014)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q014A, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 92700, true, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q014A)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q015, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 110800, false, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q015)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q015A, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 110800, true, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q015A)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q016, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 129100, false, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q016)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q016A, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 129100, true, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q016A)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q017, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 147300, false, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q017)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q017A, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 147300, true, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q017A)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q018, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 165500, false, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q018)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q018A, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 165500, true, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q018A)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q019, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 184000, false, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q019)

DUCKDB_BENCHMARK(LDBC_MICRO04_Q019A, "[ldbc_micro_04]")
LDBC_QUERY_BODY(4, 184000, true, "MICRO04")
FINISH_BENCHMARK(LDBC_MICRO04_Q019A)
