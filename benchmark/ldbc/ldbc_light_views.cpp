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
		return ldbc::get_light_query_with_views(QNR, SF);                                                              \
	}                                                                                                                  \
	virtual string GetJO() {                                                                                           \
		return ldbc::get_views_jo(JO_NAME);                                                                            \
	}                                                                                                                  \
	virtual string GetViews() {                                                                                        \
		return ldbc::get_views(QNR);                                                                                   \
	}                                                                                                                  \
	virtual string VerifyResult(QueryResult *result) {                                                                 \
		if (!result->success) {                                                                                        \
			return result->error;                                                                                      \
		}                                                                                                              \
		return "";                                                                                                     \
	}                                                                                                                  \
	virtual string BenchmarkInfo() {                                                                                   \
		return StringUtil::Format("LDBC Light Q%d: %s", QNR, ldbc::get_light_query_with_views(QNR, SF).c_str());       \
	}

// IS1
DUCKDB_BENCHMARK(SNB_M_VIEWS_IS1, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("is01", false, "EMPTY");
FINISH_BENCHMARK(SNB_M_VIEWS_IS1)

// IS2
DUCKDB_BENCHMARK(SNB_M_VIEWS_IS2, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("is02", false, "is02");
FINISH_BENCHMARK(SNB_M_VIEWS_IS2)

// IS3
DUCKDB_BENCHMARK(SNB_M_VIEWS_IS3, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("is03", false, "is03");
FINISH_BENCHMARK(SNB_M_VIEWS_IS3)

// IS5
DUCKDB_BENCHMARK(SNB_M_VIEWS_IS5, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("is05", false, "EMPTY");
FINISH_BENCHMARK(SNB_M_VIEWS_IS5)

// IS6
DUCKDB_BENCHMARK(SNB_M_VIEWS_IS6, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("is06", false, "is06");
FINISH_BENCHMARK(SNB_M_VIEWS_IS6)

// IS7
DUCKDB_BENCHMARK(SNB_M_VIEWS_IS7, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("is07", false, "is07");
FINISH_BENCHMARK(SNB_M_VIEWS_IS7)

// IC1-1
DUCKDB_BENCHMARK(SNB_M_VIEWS_IC1_1, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("ic01a", false, "ic01a");
FINISH_BENCHMARK(SNB_M_VIEWS_IC1_1)

// IC1-2
DUCKDB_BENCHMARK(SNB_M_VIEWS_IC1_2, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("ic01b", false, "ic01b");
FINISH_BENCHMARK(SNB_M_VIEWS_IC1_2)

// IC1-3
DUCKDB_BENCHMARK(SNB_M_VIEWS_IC1_3, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("ic01c", false, "ic01c");
FINISH_BENCHMARK(SNB_M_VIEWS_IC1_3)

// IC2
DUCKDB_BENCHMARK(SNB_M_VIEWS_IC2, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("ic02", false, "ic02");
FINISH_BENCHMARK(SNB_M_VIEWS_IC2)

// IC3-1
DUCKDB_BENCHMARK(SNB_M_VIEWS_IC3_1, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("ic03a", false, "ic03a");
FINISH_BENCHMARK(SNB_M_VIEWS_IC3_1)

// IC3-2
DUCKDB_BENCHMARK(SNB_M_VIEWS_IC3_2, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("ic03b", false, "ic03b");
FINISH_BENCHMARK(SNB_M_VIEWS_IC3_2)

// IC4
DUCKDB_BENCHMARK(SNB_M_VIEWS_IC4, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("ic04", false, "ic04");
FINISH_BENCHMARK(SNB_M_VIEWS_IC4)

// IC5-1
DUCKDB_BENCHMARK(SNB_M_VIEWS_IC5_1, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("ic05a", false, "ic05a");
FINISH_BENCHMARK(SNB_M_VIEWS_IC5_1)

// IC5-2
DUCKDB_BENCHMARK(SNB_M_VIEWS_IC5_2, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("ic05b", false, "ic05b");
FINISH_BENCHMARK(SNB_M_VIEWS_IC5_2)

// IC6-1
DUCKDB_BENCHMARK(SNB_M_VIEWS_IC6_1, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("ic06a", false, "ic06a");
FINISH_BENCHMARK(SNB_M_VIEWS_IC6_1)

// IC6-2
DUCKDB_BENCHMARK(SNB_M_VIEWS_IC6_2, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("ic06b", false, "ic06b");
FINISH_BENCHMARK(SNB_M_VIEWS_IC6_2)

// IC7
DUCKDB_BENCHMARK(SNB_M_VIEWS_IC7, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("ic07", false, "ic07");
FINISH_BENCHMARK(SNB_M_VIEWS_IC7)

// IC8
DUCKDB_BENCHMARK(SNB_M_VIEWS_IC8, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("ic08", false, "ic08");
FINISH_BENCHMARK(SNB_M_VIEWS_IC8)

// IC9-1
DUCKDB_BENCHMARK(SNB_M_VIEWS_IC9_1, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("ic09a", false, "ic09a");
FINISH_BENCHMARK(SNB_M_VIEWS_IC9_1)

// IC9-2
DUCKDB_BENCHMARK(SNB_M_VIEWS_IC9_2, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("ic09b", false, "ic09b");
FINISH_BENCHMARK(SNB_M_VIEWS_IC9_2)

// IC11-1
DUCKDB_BENCHMARK(SNB_M_VIEWS_IC11_1, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("ic11a", false, "ic11a");
FINISH_BENCHMARK(SNB_M_VIEWS_IC11_1)

// IC11-2
DUCKDB_BENCHMARK(SNB_M_VIEWS_IC11_2, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("ic11b", false, "ic11b");
FINISH_BENCHMARK(SNB_M_VIEWS_IC11_2)

// IC12-2
DUCKDB_BENCHMARK(SNB_M_VIEWS_IC12, "[SNB_M_VIEWS]")
LDBC_QUERY_BODY("ic12", false, "ic12");
FINISH_BENCHMARK(SNB_M_VIEWS_IC12)
