#include "benchmark_runner.hpp"
#include "compare_result.hpp"
#include "dbgen.hpp"
#include "duckdb_benchmark_macro.hpp"

using namespace duckdb;
using namespace std;

#define SF 10

#define TPCH_RAI_STMT                                                                                                  \
	"CREATE PKFK RAI supplierInNation ON supplier (FROM s_nationkey REFERENCES nation.n_nationkey, TO s_nationkey "    \
	"REFERENCES nation.n_nationkey);"                                                                                  \
	"CREATE PKFK RAI customerInNation ON customer (FROM c_nationkey REFERENCES nation.n_nationkey, TO c_nationkey "    \
	"REFERENCES nation.n_nationkey);"                                                                                  \
	"CREATE PKFK RAI customerHasOrders ON orders (FROM o_custkey REFERENCES customer.c_custkey, TO o_custkey "         \
	"REFERENCES customer.c_custkey);"                                                                                  \
	"CREATE PKFK RAI ordersHasLineitems ON lineitem (FROM l_orderkey REFERENCES orders.o_orderkey, TO l_orderkey "     \
	"REFERENCES orders.o_orderkey);"                                                                                   \
	"CREATE UNDIRECTED RAI order_part ON lineitem (FROM l_orderkey REFERENCES orders.o_orderkey, TO l_partkey "        \
	"REFERENCES part.p_partkey);"                                                                                      \
	"CREATE UNDIRECTED RAI order_supp ON lineitem (FROM l_orderkey REFERENCES orders.o_orderkey, TO l_suppkey "        \
	"REFERENCES supplier.s_suppkey);"                                                                                  \
	"CREATE UNDIRECTED RAI supp_part ON partsupp (FROM ps_suppkey REFERENCES supplier.s_suppkey, TO ps_partkey "       \
	"REFERENCES part.p_partkey);"

#define TPCH_QUERY_BODY(QNR, ENABLE_RAIS, JO)                                                                          \
	virtual void Load(DuckDBBenchmarkState *state) {                                                                   \
		if (!BenchmarkRunner::TryLoadDatabase(state->db, "tpch", ENABLE_RAIS, TPCH_RAI_STMT)) {                        \
			tpch::dbgen(SF, state->db, ENABLE_RAIS);                                                                   \
			BenchmarkRunner::SaveDatabase(state->db, "tpch");                                                          \
		}                                                                                                              \
	}                                                                                                                  \
	virtual string GetQuery() {                                                                                        \
		return tpch::get_query(QNR);                                                                                   \
	}                                                                                                                  \
	virtual string GetJO() {                                                                                           \
		return tpch::get_default_jo(JO);                                                                               \
	}                                                                                                                  \
	virtual string VerifyResult(QueryResult *result) {                                                                 \
		if (!result->success) {                                                                                        \
			return result->error;                                                                                      \
		}                                                                                                              \
		return "";                                                                                                     \
	}                                                                                                                  \
	virtual string BenchmarkInfo() {                                                                                   \
		return StringUtil::Format("TPC-H Q%d SF%d: %s", QNR, SF, tpch::get_query(QNR).c_str());                        \
	}

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q01, "[tpch-default]");
TPCH_QUERY_BODY(1, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q01);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q01A, "[tpch-default]");
TPCH_QUERY_BODY(1, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q01A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q02, "[tpch-default]");
TPCH_QUERY_BODY(2, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q02);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q02A, "[tpch-default]");
TPCH_QUERY_BODY(2, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q02A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q03, "[tpch-default]");
TPCH_QUERY_BODY(3, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q03);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q03A, "[tpch-default]");
TPCH_QUERY_BODY(3, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q03A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q04, "[tpch-default]");
TPCH_QUERY_BODY(4, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q04);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q04A, "[tpch-default]");
TPCH_QUERY_BODY(4, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q04A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q05, "[tpch-default]");
TPCH_QUERY_BODY(5, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q05);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q05A, "[tpch-default]");
TPCH_QUERY_BODY(5, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q05A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q06, "[tpch-default]");
TPCH_QUERY_BODY(6, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q06);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q06A, "[tpch-default]");
TPCH_QUERY_BODY(6, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q06A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q07, "[tpch-default]");
TPCH_QUERY_BODY(7, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q07);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q07A, "[tpch-default]");
TPCH_QUERY_BODY(7, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q07A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q08, "[tpch-default]");
TPCH_QUERY_BODY(8, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q08);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q08A, "[tpch-default]");
TPCH_QUERY_BODY(8, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q08A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q09, "[tpch-default]");
TPCH_QUERY_BODY(9, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q09);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q09A, "[tpch-default]");
TPCH_QUERY_BODY(9, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q09A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q10, "[tpch-default]");
TPCH_QUERY_BODY(10, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q10);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q10A, "[tpch-default]");
TPCH_QUERY_BODY(10, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q10A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q11, "[tpch-default]");
TPCH_QUERY_BODY(11, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q11);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q11A, "[tpch-default]");
TPCH_QUERY_BODY(11, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q11A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q12, "[tpch-default]");
TPCH_QUERY_BODY(12, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q12);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q12A, "[tpch-default]");
TPCH_QUERY_BODY(12, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q12A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q13, "[tpch-default]");
TPCH_QUERY_BODY(13, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q13);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q13A, "[tpch-default]");
TPCH_QUERY_BODY(13, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q13A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q14, "[tpch-default]");
TPCH_QUERY_BODY(14, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q14);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q14A, "[tpch-default]");
TPCH_QUERY_BODY(14, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q14A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q15, "[tpch-default]");
TPCH_QUERY_BODY(15, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q15);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q15A, "[tpch-default]");
TPCH_QUERY_BODY(15, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q15A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q16, "[tpch-default]");
TPCH_QUERY_BODY(16, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q16);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q16A, "[tpch-default]");
TPCH_QUERY_BODY(16, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q16A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q17, "[tpch-default]");
TPCH_QUERY_BODY(17, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q17);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q17A, "[tpch-default]");
TPCH_QUERY_BODY(17, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q17A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q18, "[tpch-default]");
TPCH_QUERY_BODY(18, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q18);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q18A, "[tpch-default]");
TPCH_QUERY_BODY(18, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q18A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q19, "[tpch-default]");
TPCH_QUERY_BODY(19, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q19);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q19A, "[tpch-default]");
TPCH_QUERY_BODY(19, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q19A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q20, "[tpch-default]");
TPCH_QUERY_BODY(20, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q20);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q20A, "[tpch-default]");
TPCH_QUERY_BODY(20, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q20A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q21, "[tpch-default]");
TPCH_QUERY_BODY(21, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q21);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q21A, "[tpch-default]");
TPCH_QUERY_BODY(21, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q21A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q22, "[tpch-default]");
TPCH_QUERY_BODY(22, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q22);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q22A, "[tpch-default]");
TPCH_QUERY_BODY(22, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q22A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q23, "[tpch-default]");
TPCH_QUERY_BODY(23, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q23);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q23A, "[tpch-default]");
TPCH_QUERY_BODY(23, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q23A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q24, "[tpch-default]");
TPCH_QUERY_BODY(24, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q24);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q24A, "[tpch-default]");
TPCH_QUERY_BODY(24, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q24A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q25, "[tpch-default]");
TPCH_QUERY_BODY(25, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q25);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q25A, "[tpch-default]");
TPCH_QUERY_BODY(25, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q25A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q26, "[tpch-default]");
TPCH_QUERY_BODY(26, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q26);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q26A, "[tpch-default]");
TPCH_QUERY_BODY(26, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q26A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q27, "[tpch-default]");
TPCH_QUERY_BODY(27, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q27);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q27A, "[tpch-default]");
TPCH_QUERY_BODY(27, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q27A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q28, "[tpch-default]");
TPCH_QUERY_BODY(28, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q28);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q28A, "[tpch-default]");
TPCH_QUERY_BODY(28, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q28A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q29, "[tpch-default]");
TPCH_QUERY_BODY(29, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q29);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q29A, "[tpch-default]");
TPCH_QUERY_BODY(29, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q29A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q30, "[tpch-default]");
TPCH_QUERY_BODY(30, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q30);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q30A, "[tpch-default]");
TPCH_QUERY_BODY(30, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q30A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q31, "[tpch-default]");
TPCH_QUERY_BODY(31, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q31);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q31A, "[tpch-default]");
TPCH_QUERY_BODY(31, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q31A);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q32, "[tpch-default]");
TPCH_QUERY_BODY(32, false, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q32);

DUCKDB_BENCHMARK(TPCH_DEFAULT_Q32A, "[tpch-default]");
TPCH_QUERY_BODY(32, true, "EMPTY");
FINISH_BENCHMARK(TPCH_DEFAULT_Q32A);
