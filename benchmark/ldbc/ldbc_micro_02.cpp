#include "benchmark_runner.hpp"
#include "compare_result.hpp"
#include "duckdb_benchmark_macro.hpp"
#include "ldbc.hpp"

using namespace duckdb;
using namespace std;

#define SF 30

#define LDBC_QUERY_BODY(QNR, PARAM, ENABLE_RAIS, JO_NAME)                                                              \
	virtual void Load(DuckDBBenchmarkState *state) {                                                                   \
		ldbc::dbgen_micro(state->conn, SF, 2, ENABLE_RAIS);                                                            \
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

DUCKDB_BENCHMARK(LDBC_MICRO02_Q001, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 184, false, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q001)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q001A, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 184, true, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q001A)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q002, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 367, false, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q002)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q002A, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 367, true, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q002A)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q003, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 918, false, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q003)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q003A, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 918, true, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q003A)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q004, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 1835, false, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q004)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q004A, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 1835, true, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q004A)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q005, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 3671, false, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q005)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q005A, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 3671, true, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q005A)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q006, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 9177, false, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q006)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q006A, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 9177, true, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q006A)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q007, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 18355, false, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q007)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q007A, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 18355, true, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q007A)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q008, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 36709, false, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q008)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q008A, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 36709, true, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q008A)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q009, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 91773, false, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q009)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q009A, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 91773, true, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q009A)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q010, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 183546, false, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q010)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q010A, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 183546, true, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q010A)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q011, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 367092, false, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q011)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q011A, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 367092, true, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q011A)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q012, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 550637, false, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q012)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q012A, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 550637, true, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q012A)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q013, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 734183, false, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q013)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q013A, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 734183, true, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q013A)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q014, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 917729, false, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q014)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q014A, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 917729, true, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q014A)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q015, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 1101275, false, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q015)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q015A, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 1101275, true, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q015A)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q016, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 1284821, false, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q016)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q016A, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 1284821, true, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q016A)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q017, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 1468366, false, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q017)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q017A, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 1468366, true, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q017A)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q018, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 1651912, false, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q018)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q018A, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 1651912, true, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q018A)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q019, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 1835458, false, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q019)

DUCKDB_BENCHMARK(LDBC_MICRO02_Q019A, "[ldbc_micro_02]")
LDBC_QUERY_BODY(2, 1835458, true, "MICRO02")
FINISH_BENCHMARK(LDBC_MICRO02_Q019A)
