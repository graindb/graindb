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

DUCKDB_BENCHMARK(LDBC_MICROK_Q001, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1263736391, false, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q001)

DUCKDB_BENCHMARK(LDBC_MICROK_Q001A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1263736391, true, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q001A)

DUCKDB_BENCHMARK(LDBC_MICROK_Q002, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1264096391, false, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q002)

DUCKDB_BENCHMARK(LDBC_MICROK_Q002A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1264096391, true, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q002A)

DUCKDB_BENCHMARK(LDBC_MICROK_Q003, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1264736391, false, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q003)

DUCKDB_BENCHMARK(LDBC_MICROK_Q003A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1264736391, true, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q003A)

DUCKDB_BENCHMARK(LDBC_MICROK_Q004, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1265366391, false, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q004)

DUCKDB_BENCHMARK(LDBC_MICROK_Q004A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1265366391, true, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q004A)

DUCKDB_BENCHMARK(LDBC_MICROK_Q005, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1266216391, false, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q005)

DUCKDB_BENCHMARK(LDBC_MICROK_Q005A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1266216391, true, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q005A)

DUCKDB_BENCHMARK(LDBC_MICROK_Q006, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1267916391, false, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q006)

DUCKDB_BENCHMARK(LDBC_MICROK_Q006A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1267916391, true, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q006A)

DUCKDB_BENCHMARK(LDBC_MICROK_Q007, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1270216391, false, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q007)

DUCKDB_BENCHMARK(LDBC_MICROK_Q007A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1270216391, true, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q007A)

DUCKDB_BENCHMARK(LDBC_MICROK_Q008, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1273536391, false, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q008)

DUCKDB_BENCHMARK(LDBC_MICROK_Q008A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1273536391, true, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q008A)

DUCKDB_BENCHMARK(LDBC_MICROK_Q009, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1280516391, false, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q009)

DUCKDB_BENCHMARK(LDBC_MICROK_Q009A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1280516391, true, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q009A)

DUCKDB_BENCHMARK(LDBC_MICROK_Q010, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1288866391, false, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q010)

DUCKDB_BENCHMARK(LDBC_MICROK_Q010A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1288866391, true, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q010A)

DUCKDB_BENCHMARK(LDBC_MICROK_Q011, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1301606391, false, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q011)

DUCKDB_BENCHMARK(LDBC_MICROK_Q011A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1301606391, true, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q011A)

DUCKDB_BENCHMARK(LDBC_MICROK_Q012, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1311766391, false, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q012)

DUCKDB_BENCHMARK(LDBC_MICROK_Q012A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1311766391, true, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q012A)

DUCKDB_BENCHMARK(LDBC_MICROK_Q013, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1320486391, false, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q013)

DUCKDB_BENCHMARK(LDBC_MICROK_Q013A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1320486391, true, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q013A)

DUCKDB_BENCHMARK(LDBC_MICROK_Q014, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1328406391, false, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q014)

DUCKDB_BENCHMARK(LDBC_MICROK_Q014A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1328406391, true, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q014A)

DUCKDB_BENCHMARK(LDBC_MICROK_Q015, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1335216391, false, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q015)

DUCKDB_BENCHMARK(LDBC_MICROK_Q015A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1335216391, true, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q015A)

DUCKDB_BENCHMARK(LDBC_MICROK_Q016, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1341616391, false, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q016)

DUCKDB_BENCHMARK(LDBC_MICROK_Q016A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1341616391, true, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q016A)

DUCKDB_BENCHMARK(LDBC_MICROK_Q017, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1347496391, false, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q017)

DUCKDB_BENCHMARK(LDBC_MICROK_Q017A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1347496391, true, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q017A)

DUCKDB_BENCHMARK(LDBC_MICROK_Q018, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1353146391, false, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q018)

DUCKDB_BENCHMARK(LDBC_MICROK_Q018A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1353146391, true, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q018A)

DUCKDB_BENCHMARK(LDBC_MICROK_Q019, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1400000000, false, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q019)

DUCKDB_BENCHMARK(LDBC_MICROK_Q019A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1400000000, true, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q019A)
