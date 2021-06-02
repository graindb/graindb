#include "benchmark_runner.hpp"
#include "compare_result.hpp"
#include "duckdb_benchmark_macro.hpp"
#include "ldbc.hpp"

using namespace duckdb;
using namespace std;

#define SF 30

#define LDBC_QUERY_BODY(QNR, PARAM, ENABLE_RAIS, JO_NAME)                                                              \
	virtual void Load(DuckDBBenchmarkState *state) {                                                                   \
		ldbc::dbgen_micro(state->conn, SF, 9, ENABLE_RAIS);                                                            \
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

DUCKDB_BENCHMARK(LDBC_MICRO09_Q001, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1263736391, false, "MICRO09A")
FINISH_BENCHMARK(LDBC_MICRO09_Q001)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q001A, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1263736391, true, "MICRO09A")
FINISH_BENCHMARK(LDBC_MICRO09_Q001A)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q002, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1264096391, false, "MICRO09A")
FINISH_BENCHMARK(LDBC_MICRO09_Q002)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q002A, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1264096391, true, "MICRO09A")
FINISH_BENCHMARK(LDBC_MICRO09_Q002A)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q003, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1264736391, false, "MICRO09A")
FINISH_BENCHMARK(LDBC_MICRO09_Q003)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q003A, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1264736391, true, "MICRO09A")
FINISH_BENCHMARK(LDBC_MICRO09_Q003A)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q004, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1265366391, false, "MICRO09A")
FINISH_BENCHMARK(LDBC_MICRO09_Q004)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q004A, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1265366391, true, "MICRO09A")
FINISH_BENCHMARK(LDBC_MICRO09_Q004A)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q005, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1266216391, false, "MICRO09A")
FINISH_BENCHMARK(LDBC_MICRO09_Q005)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q005A, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1266216391, true, "MICRO09A")
FINISH_BENCHMARK(LDBC_MICRO09_Q005A)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q006, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1267916391, false, "MICRO09A")
FINISH_BENCHMARK(LDBC_MICRO09_Q006)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q006A, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1267916391, true, "MICRO09A")
FINISH_BENCHMARK(LDBC_MICRO09_Q006A)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q007, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1270216391, false, "MICRO09A")
FINISH_BENCHMARK(LDBC_MICRO09_Q007)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q007A, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1270216391, true, "MICRO09A")
FINISH_BENCHMARK(LDBC_MICRO09_Q007A)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q008, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1273536391, false, "MICRO09A")
FINISH_BENCHMARK(LDBC_MICRO09_Q008)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q008A, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1273536391, true, "MICRO09A")
FINISH_BENCHMARK(LDBC_MICRO09_Q008A)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q009, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1280516391, false, "MICRO09")
FINISH_BENCHMARK(LDBC_MICRO09_Q009)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q009A, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1280516391, true, "MICRO09")
FINISH_BENCHMARK(LDBC_MICRO09_Q009A)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q010, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1288866391, false, "MICRO09")
FINISH_BENCHMARK(LDBC_MICRO09_Q010)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q010A, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1288866391, true, "MICRO09")
FINISH_BENCHMARK(LDBC_MICRO09_Q010A)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q011, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1301606391, false, "MICRO09")
FINISH_BENCHMARK(LDBC_MICRO09_Q011)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q011A, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1301606391, true, "MICRO09")
FINISH_BENCHMARK(LDBC_MICRO09_Q011A)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q012, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1311766391, false, "MICRO09")
FINISH_BENCHMARK(LDBC_MICRO09_Q012)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q012A, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1311766391, true, "MICRO09")
FINISH_BENCHMARK(LDBC_MICRO09_Q012A)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q013, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1320486391, false, "MICRO09")
FINISH_BENCHMARK(LDBC_MICRO09_Q013)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q013A, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1320486391, true, "MICRO09")
FINISH_BENCHMARK(LDBC_MICRO09_Q013A)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q014, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1328406391, false, "MICRO09")
FINISH_BENCHMARK(LDBC_MICRO09_Q014)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q014A, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1328406391, true, "MICRO09")
FINISH_BENCHMARK(LDBC_MICRO09_Q014A)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q015, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1335216391, false, "MICRO09")
FINISH_BENCHMARK(LDBC_MICRO09_Q015)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q015A, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1335216391, true, "MICRO09")
FINISH_BENCHMARK(LDBC_MICRO09_Q015A)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q016, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1341616391, false, "MICRO09")
FINISH_BENCHMARK(LDBC_MICRO09_Q016)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q016A, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1341616391, true, "MICRO09")
FINISH_BENCHMARK(LDBC_MICRO09_Q016A)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q017, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1347496391, false, "MICRO09")
FINISH_BENCHMARK(LDBC_MICRO09_Q017)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q017A, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1347496391, true, "MICRO09")
FINISH_BENCHMARK(LDBC_MICRO09_Q017A)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q018, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1353146391, false, "MICRO09")
FINISH_BENCHMARK(LDBC_MICRO09_Q018)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q018A, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1353146391, true, "MICRO09")
FINISH_BENCHMARK(LDBC_MICRO09_Q018A)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q019, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1400000000, false, "MICRO09")
FINISH_BENCHMARK(LDBC_MICRO09_Q019)

DUCKDB_BENCHMARK(LDBC_MICRO09_Q019A, "[ldbc_micro_09]")
LDBC_QUERY_BODY(9, 1400000000, true, "MICRO09")
FINISH_BENCHMARK(LDBC_MICRO09_Q019A)
