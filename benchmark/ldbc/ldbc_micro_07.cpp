#include "benchmark_runner.hpp"
#include "compare_result.hpp"
#include "duckdb_benchmark_macro.hpp"
#include "ldbc.hpp"

using namespace duckdb;
using namespace std;

#define SF 30

#define LDBC_QUERY_BODY(QNR, PARAM, ENABLE_RAIS, JO_NAME)                                                              \
	virtual void Load(DuckDBBenchmarkState *state) {                                                                   \
		ldbc::dbgen_micro(state->conn, SF, 7, ENABLE_RAIS);                                                            \
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

DUCKDB_BENCHMARK(LDBC_MICRO07_Q001, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1266955328, false, "MICRO07A")
FINISH_BENCHMARK(LDBC_MICRO07_Q001)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q001A, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1266955328, true, "MICRO07A")
FINISH_BENCHMARK(LDBC_MICRO07_Q001A)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q002, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1267275328, false, "MICRO07A")
FINISH_BENCHMARK(LDBC_MICRO07_Q002)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q002A, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1267275328, true, "MICRO07A")
FINISH_BENCHMARK(LDBC_MICRO07_Q002A)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q003, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1268575328, false, "MICRO07A")
FINISH_BENCHMARK(LDBC_MICRO07_Q003)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q003A, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1268575328, true, "MICRO07A")
FINISH_BENCHMARK(LDBC_MICRO07_Q003A)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q004, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1270685328, false, "MICRO07A")
FINISH_BENCHMARK(LDBC_MICRO07_Q004)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q004A, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1270685328, true, "MICRO07A")
FINISH_BENCHMARK(LDBC_MICRO07_Q004A)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q005, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1272765328, false, "MICRO07A")
FINISH_BENCHMARK(LDBC_MICRO07_Q005)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q005A, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1272765328, true, "MICRO07A")
FINISH_BENCHMARK(LDBC_MICRO07_Q005A)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q006, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1275965328, false, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q006)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q006A, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1275965328, true, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q006A)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q007, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1280295328, false, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q007)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q007A, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1280295328, true, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q007A)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q008, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1283795328, false, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q008)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q008A, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1283795328, true, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q008A)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q009, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1291795328, false, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q009)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q009A, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1291795328, true, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q009A)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q010, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1299795328, false, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q010)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q010A, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1299795328, true, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q010A)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q011, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1312795328, false, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q011)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q011A, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1312795328, true, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q011A)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q012, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1319795328, false, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q012)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q012A, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1319795328, true, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q012A)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q013, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1326295328, false, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q013)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q013A, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1326295328, true, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q013A)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q014, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1332795328, false, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q014)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q014A, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1332795328, true, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q014A)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q015, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1337795328, false, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q015)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q015A, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1337795328, true, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q015A)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q016, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1342795328, false, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q016)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q016A, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1342795328, true, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q016A)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q017, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1348295328, false, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q017)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q017A, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1348295328, true, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q017A)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q018, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1353295328, false, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q018)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q018A, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1353295328, true, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q018A)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q019, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1364568245, false, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q019)

DUCKDB_BENCHMARK(LDBC_MICRO07_Q019A, "[ldbc_micro_07]")
LDBC_QUERY_BODY(7, 1364568245, true, "MICRO07")
FINISH_BENCHMARK(LDBC_MICRO07_Q019A)
