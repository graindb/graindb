#include "benchmark_runner.hpp"
#include "compare_result.hpp"
#include "duckdb_benchmark_macro.hpp"
#include "ldbc.hpp"

using namespace duckdb;
using namespace std;

#define SF 10

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

DUCKDB_BENCHMARK(LDBC_MICROP_Q001, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 20, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q001)

DUCKDB_BENCHMARK(LDBC_MICROP_Q001A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 20, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q001A)

DUCKDB_BENCHMARK(LDBC_MICROP_Q002, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 40, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q002)

DUCKDB_BENCHMARK(LDBC_MICROP_Q002A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 40, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q002A)

DUCKDB_BENCHMARK(LDBC_MICROP_Q003, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 80, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q003)

DUCKDB_BENCHMARK(LDBC_MICROP_Q003A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 80, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q003A)

DUCKDB_BENCHMARK(LDBC_MICROP_Q004, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 150, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q004)

DUCKDB_BENCHMARK(LDBC_MICROP_Q004A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 150, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q004A)

DUCKDB_BENCHMARK(LDBC_MICROP_Q005, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 270, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q005)

DUCKDB_BENCHMARK(LDBC_MICROP_Q005A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 270, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q005A)

DUCKDB_BENCHMARK(LDBC_MICROP_Q006, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 640, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q006)

DUCKDB_BENCHMARK(LDBC_MICROP_Q006A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 640, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q006A)

DUCKDB_BENCHMARK(LDBC_MICROP_Q007, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 2000, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q007)

DUCKDB_BENCHMARK(LDBC_MICROP_Q007A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 2000, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q007A)

DUCKDB_BENCHMARK(LDBC_MICROP_Q008, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 3800, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q008)

DUCKDB_BENCHMARK(LDBC_MICROP_Q008A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 3800, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q008A)

DUCKDB_BENCHMARK(LDBC_MICROP_Q009, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 9300, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q009)

DUCKDB_BENCHMARK(LDBC_MICROP_Q009A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 9300, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q009A)

DUCKDB_BENCHMARK(LDBC_MICROP_Q010, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 18700, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q010)

DUCKDB_BENCHMARK(LDBC_MICROP_Q010A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 18700, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q010A)

DUCKDB_BENCHMARK(LDBC_MICROP_Q011, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 37600, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q011)

DUCKDB_BENCHMARK(LDBC_MICROP_Q011A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 37600, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q011A)

DUCKDB_BENCHMARK(LDBC_MICROP_Q012, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 55700, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q012)

DUCKDB_BENCHMARK(LDBC_MICROP_Q012A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 55700, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q012A)

DUCKDB_BENCHMARK(LDBC_MICROP_Q013, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 74200, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q013)

DUCKDB_BENCHMARK(LDBC_MICROP_Q013A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 74200, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q013A)

DUCKDB_BENCHMARK(LDBC_MICROP_Q014, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 92700, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q014)

DUCKDB_BENCHMARK(LDBC_MICROP_Q014A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 92700, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q014A)

DUCKDB_BENCHMARK(LDBC_MICROP_Q015, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 110800, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q015)

DUCKDB_BENCHMARK(LDBC_MICROP_Q015A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 110800, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q015A)

DUCKDB_BENCHMARK(LDBC_MICROP_Q016, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 129100, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q016)

DUCKDB_BENCHMARK(LDBC_MICROP_Q016A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 129100, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q016A)

DUCKDB_BENCHMARK(LDBC_MICROP_Q017, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 147300, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q017)

DUCKDB_BENCHMARK(LDBC_MICROP_Q017A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 147300, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q017A)

DUCKDB_BENCHMARK(LDBC_MICROP_Q018, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 165500, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q018)

DUCKDB_BENCHMARK(LDBC_MICROP_Q018A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 165500, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q018A)

DUCKDB_BENCHMARK(LDBC_MICROP_Q019, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 184000, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q019)

DUCKDB_BENCHMARK(LDBC_MICROP_Q019A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 184000, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q019A)
