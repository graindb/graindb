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
		return ldbc::get_default_jo(JO_NAME);                                                                          \
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
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q001, "[ldbc_light_default]")
LDBC_QUERY_BODY(1, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q001)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q001A, "[ldbc_light_default]")
LDBC_QUERY_BODY(1, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q001A)

// ic01b
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q002, "[ldbc_light_default]")
LDBC_QUERY_BODY(2, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q002)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q002A, "[ldbc_light_default]")
LDBC_QUERY_BODY(2, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q002A)

// ic01c
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q003, "[ldbc_light_default]")
LDBC_QUERY_BODY(3, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q003)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q003A, "[ldbc_light_default]")
LDBC_QUERY_BODY(3, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q003A)

// ic02
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q004, "[ldbc_light_default]")
LDBC_QUERY_BODY(4, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q004)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q004A, "[ldbc_light_default]")
LDBC_QUERY_BODY(4, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q004A)

// ic03a
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q005, "[ldbc_light_default]")
LDBC_QUERY_BODY(5, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q005)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q005A, "[ldbc_light_default]")
LDBC_QUERY_BODY(5, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q005A)

// ic03b
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q006, "[ldbc_light_default]")
LDBC_QUERY_BODY(6, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q006)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q006A, "[ldbc_light_default]")
LDBC_QUERY_BODY(6, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q006A)

// ic04
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q007, "[ldbc_light_default]")
LDBC_QUERY_BODY(7, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q007)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q007A, "[ldbc_light_default]")
LDBC_QUERY_BODY(7, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q007A)

// ic05a
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q008, "[ldbc_light_default]")
LDBC_QUERY_BODY(8, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q008)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q008A, "[ldbc_light_default]")
LDBC_QUERY_BODY(8, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q008A)

// ic05b
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q009, "[ldbc_light_default]")
LDBC_QUERY_BODY(9, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q009)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q009A, "[ldbc_light_default]")
LDBC_QUERY_BODY(9, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q009A)

// ic06a
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q010, "[ldbc_light_default]")
LDBC_QUERY_BODY(10, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q010)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q010A, "[ldbc_light_default]")
LDBC_QUERY_BODY(10, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q010A)

// ic06b
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q011, "[ldbc_light_default]")
LDBC_QUERY_BODY(11, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q011)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q011A, "[ldbc_light_default]")
LDBC_QUERY_BODY(11, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q011A)

// ic07
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q012, "[ldbc_light_default]")
LDBC_QUERY_BODY(12, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q012)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q012A, "[ldbc_light_default]")
LDBC_QUERY_BODY(12, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q012A)

// ic08
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q013, "[ldbc_light_default]")
LDBC_QUERY_BODY(13, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q013)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q013A, "[ldbc_light_default]")
LDBC_QUERY_BODY(13, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q013A)

// ic09a
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q014, "[ldbc_light_default]")
LDBC_QUERY_BODY(14, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q014)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q014A, "[ldbc_light_default]")
LDBC_QUERY_BODY(14, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q014A)

// ic09b
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q015, "[ldbc_light_default]")
LDBC_QUERY_BODY(15, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q015)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q015A, "[ldbc_light_default]")
LDBC_QUERY_BODY(15, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q015A)

// ic10
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q016, "[ldbc_light_default]")
LDBC_QUERY_BODY(16, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q016)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q016A, "[ldbc_light_default]")
LDBC_QUERY_BODY(16, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q016A)

// ic11a
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q017, "[ldbc_light_default]")
LDBC_QUERY_BODY(17, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q017)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q017A, "[ldbc_light_default]")
LDBC_QUERY_BODY(17, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q017A)

// ic11b
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q018, "[ldbc_light_default]")
LDBC_QUERY_BODY(18, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q018)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q018A, "[ldbc_light_default]")
LDBC_QUERY_BODY(18, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q018A)

// is01
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q019, "[ldbc_light_default]")
LDBC_QUERY_BODY(19, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q019)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q019A, "[ldbc_light_default]")
LDBC_QUERY_BODY(19, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q019A)

// is02
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q020, "[ldbc_light_default]")
LDBC_QUERY_BODY(20, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q020)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q020A, "[ldbc_light_default]")
LDBC_QUERY_BODY(20, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q020A)

// is03
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q021, "[ldbc_light_default]")
LDBC_QUERY_BODY(21, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q021)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q021A, "[ldbc_light_default]")
LDBC_QUERY_BODY(21, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q021A)

// is04
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q022, "[ldbc_light_default]")
LDBC_QUERY_BODY(22, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q022)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q022A, "[ldbc_light_default]")
LDBC_QUERY_BODY(22, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q022A)

// is05
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q023, "[ldbc_light_default]")
LDBC_QUERY_BODY(23, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q023)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q023A, "[ldbc_light_default]")
LDBC_QUERY_BODY(23, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q023A)

// is06
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q024, "[ldbc_light_default]")
LDBC_QUERY_BODY(24, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q024)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q024A, "[ldbc_light_default]")
LDBC_QUERY_BODY(24, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q024A)

// is07
DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q025, "[ldbc_light_default]")
LDBC_QUERY_BODY(25, false, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q025)

DUCKDB_BENCHMARK(LDBC_LIGHT_DEFAULT_Q025A, "[ldbc_light_default]")
LDBC_QUERY_BODY(25, true, "EMPTY");
FINISH_BENCHMARK(LDBC_LIGHT_DEFAULT_Q025A)
