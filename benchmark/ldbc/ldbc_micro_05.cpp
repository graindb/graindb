#include "benchmark_runner.hpp"
#include "compare_result.hpp"
#include "duckdb_benchmark_macro.hpp"
#include "ldbc.hpp"

using namespace duckdb;
using namespace std;

#define SF 30

#define LDBC_QUERY_BODY(QNR, PARAM, ENABLE_RAIS, JO_NAME)                                                              \
	virtual void Load(DuckDBBenchmarkState *state) {                                                                   \
		ldbc::dbgen_micro(state->conn, SF, 5, ENABLE_RAIS);                                                            \
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

DUCKDB_BENCHMARK(LDBC_MICRO05_Q001, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 2435, false, "MICRO05A")
FINISH_BENCHMARK(LDBC_MICRO05_Q001)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q001A, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 2435, true, "MICRO05A")
FINISH_BENCHMARK(LDBC_MICRO05_Q001A)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q002, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 4869, false, "MICRO05A")
FINISH_BENCHMARK(LDBC_MICRO05_Q002)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q002A, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 4869, true, "MICRO05A")
FINISH_BENCHMARK(LDBC_MICRO05_Q002A)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q003, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 12173, false, "MICRO05A")
FINISH_BENCHMARK(LDBC_MICRO05_Q003)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q003A, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 12173, true, "MICRO05A")
FINISH_BENCHMARK(LDBC_MICRO05_Q003A)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q004, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 24346, false, "MICRO05A")
FINISH_BENCHMARK(LDBC_MICRO05_Q004)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q004A, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 24346, true, "MICRO05A")
FINISH_BENCHMARK(LDBC_MICRO05_Q004A)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q005, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 48692, false, "MICRO05A")
FINISH_BENCHMARK(LDBC_MICRO05_Q005)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q005A, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 48692, true, "MICRO05A")
FINISH_BENCHMARK(LDBC_MICRO05_Q005A)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q006, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 121731, false, "MICRO05A")
FINISH_BENCHMARK(LDBC_MICRO05_Q006)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q006A, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 121731, true, "MICRO05A")
FINISH_BENCHMARK(LDBC_MICRO05_Q006A)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q007, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 243461, false, "MICRO05A")
FINISH_BENCHMARK(LDBC_MICRO05_Q007)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q007A, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 243461, true, "MICRO05A")
FINISH_BENCHMARK(LDBC_MICRO05_Q007A)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q008, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 486922, false, "MICRO05A")
FINISH_BENCHMARK(LDBC_MICRO05_Q008)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q008A, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 486922, true, "MICRO05A")
FINISH_BENCHMARK(LDBC_MICRO05_Q008A)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q009, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 1217306, false, "MICRO05A")
FINISH_BENCHMARK(LDBC_MICRO05_Q009)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q009A, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 1217306, true, "MICRO05A")
FINISH_BENCHMARK(LDBC_MICRO05_Q009A)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q010, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 2434612, false, "MICRO05")
FINISH_BENCHMARK(LDBC_MICRO05_Q010)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q010A, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 2434612, true, "MICRO05")
FINISH_BENCHMARK(LDBC_MICRO05_Q010A)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q011, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 4869223, false, "MICRO05")
FINISH_BENCHMARK(LDBC_MICRO05_Q011)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q011A, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 4869223, true, "MICRO05")
FINISH_BENCHMARK(LDBC_MICRO05_Q011A)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q012, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 7303835, false, "MICRO05")
FINISH_BENCHMARK(LDBC_MICRO05_Q012)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q012A, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 7303835, true, "MICRO05")
FINISH_BENCHMARK(LDBC_MICRO05_Q012A)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q013, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 9738446, false, "MICRO05")
FINISH_BENCHMARK(LDBC_MICRO05_Q013)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q013A, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 9738446, true, "MICRO05")
FINISH_BENCHMARK(LDBC_MICRO05_Q013A)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q014, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 12173058, false, "MICRO05")
FINISH_BENCHMARK(LDBC_MICRO05_Q014)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q014A, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 12173058, true, "MICRO05")
FINISH_BENCHMARK(LDBC_MICRO05_Q014A)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q015, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 14607670, false, "MICRO05")
FINISH_BENCHMARK(LDBC_MICRO05_Q015)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q015A, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 14607670, true, "MICRO05")
FINISH_BENCHMARK(LDBC_MICRO05_Q015A)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q016, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 17042281, false, "MICRO05")
FINISH_BENCHMARK(LDBC_MICRO05_Q016)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q016A, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 17042281, true, "MICRO05")
FINISH_BENCHMARK(LDBC_MICRO05_Q016A)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q017, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 19476893, false, "MICRO05")
FINISH_BENCHMARK(LDBC_MICRO05_Q017)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q017A, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 19476893, true, "MICRO05")
FINISH_BENCHMARK(LDBC_MICRO05_Q017A)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q018, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 21911504, false, "MICRO05")
FINISH_BENCHMARK(LDBC_MICRO05_Q018)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q018A, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 21911504, true, "MICRO05")
FINISH_BENCHMARK(LDBC_MICRO05_Q018A)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q019, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 24346116, false, "MICRO05")
FINISH_BENCHMARK(LDBC_MICRO05_Q019)

DUCKDB_BENCHMARK(LDBC_MICRO05_Q019A, "[ldbc_micro_05]")
LDBC_QUERY_BODY(5, 24346116, true, "MICRO05")
FINISH_BENCHMARK(LDBC_MICRO05_Q019A)
