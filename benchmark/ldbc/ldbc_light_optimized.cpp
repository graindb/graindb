#include "benchmark_runner.hpp"
#include "compare_result.hpp"
#include "duckdb_benchmark_macro.hpp"
#include "ldbc.hpp"

using namespace duckdb;
using namespace std;

#define SF 10

#define LDBC_QUERY_BODY(QNR, ENABLE_RAIS, JO_NAME)                                                                     \
	virtual void Load(DuckDBBenchmarkState *state) {                                                                   \
		ldbc::dbgen(state->conn, SF, ENABLE_RAIS);                                                                     \
	}                                                                                                                  \
	virtual string GetQuery() {                                                                                        \
		return ldbc::get_light_query(QNR, SF);                                                                         \
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

// ic01a
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q001, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(1, false, "ic01a");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q001)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q001A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(1, true, "ic01a");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q001A)

// ic01b
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q002, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(2, false, "ic01b");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q002)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q002A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(2, true, "ic01b");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q002A)

// ic01c
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q003, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(3, false, "ic01c");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q003)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q003A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(3, true, "ic01c");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q003A)

// ic02
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q004, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(4, false, "ic02");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q004)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q004A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(4, true, "ic02");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q004A)

// ic03a
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q005, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(5, false, "ic03a");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q005)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q005A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(5, true, "ic03a");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q005A)

// ic03b
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q006, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(6, false, "ic03b");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q006)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q006A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(6, true, "ic03b");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q006A)

// ic04
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q007, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(7, false, "ic04");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q007)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q007A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(7, true, "ic04");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q007A)

// ic05a
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q008, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(8, false, "ic05a");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q008)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q008A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(8, true, "ic05a");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q008A)

// ic05b
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q009, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(9, false, "ic05b");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q009)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q009A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(9, true, "ic05b");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q009A)

// ic06a
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q010, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(10, false, "ic06a");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q010)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q010A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(10, true, "ic06a");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q010A)

// ic06b
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q011, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(11, false, "ic06b");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q011)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q011A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(11, true, "ic06b");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q011A)

// ic07
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q012, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(12, false, "ic07");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q012)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q012A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(12, true, "ic07");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q012A)

// ic08
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q013, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(13, false, "ic08");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q013)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q013A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(13, true, "ic08");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q013A)

// ic09a
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q014, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(14, false, "ic09a");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q014)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q014A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(14, true, "ic09a");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q014A)

// ic09b
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q015, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(15, false, "ic09b");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q015)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q015A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(15, true, "ic09b");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q015A)

// ic11a
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q017, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(17, false, "ic11a");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q017)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q017A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(17, true, "ic11a");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q017A)

// ic11b
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q018, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(18, false, "ic11b");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q018)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q018A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(18, true, "ic11b");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q018A)

// ic12
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q019, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(19, false, "ic12");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q019)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q019A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(19, true, "ic12");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q019A)

// is01
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q020, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(20, false, "is01");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q020)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q020A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(20, true, "is01");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q020A)

// is02
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q021, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(21, false, "is02");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q021)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q021A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(21, true, "is02");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q021A)

// is03
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q022, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(22, false, "is03");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q022)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q022A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(22, true, "is03");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q022A)

// is04
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q023, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(23, false, "is04");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q023)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q023A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(23, true, "is04");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q023A)

// is05
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q024, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(24, false, "is05");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q024)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q024A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(24, true, "is05");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q024A)

// is06
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q025, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(25, false, "is06");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q025)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q025A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(25, true, "is06");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q025A)

// is07
DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q026, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(26, false, "is07");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q026)

DUCKDB_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q026A, "[ldbc_light_optimized]")
LDBC_QUERY_BODY(26, true, "is07");
FINISH_BENCHMARK(LDBC_LIGHT_OPTIMIZED_Q026A)
