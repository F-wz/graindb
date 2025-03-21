#include "benchmark_runner.hpp"
#include "compare_result.hpp"
#include "dbgen.hpp"
#include "graindb_benchmark_macro.hpp"

using namespace graindb;
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
	"CREATE PKFK RAI nationHasRegion ON nation (FROM n_regionkey REFERENCES region.r_regionkey, TO n_regionkey "       \
	"REFERENCES region.r_regionkey);"                                                                                  \
	"CREATE UNDIRECTED RAI order_part ON lineitem (FROM l_orderkey REFERENCES orders.o_orderkey, TO l_partkey "        \
	"REFERENCES part.p_partkey);"                                                                                      \
	"CREATE UNDIRECTED RAI order_supp ON lineitem (FROM l_orderkey REFERENCES orders.o_orderkey, TO l_suppkey "        \
	"REFERENCES supplier.s_suppkey);"                                                                                  \
	"CREATE UNDIRECTED RAI supp_part ON partsupp (FROM ps_suppkey REFERENCES supplier.s_suppkey, TO ps_partkey "       \
	"REFERENCES part.p_partkey);"

#define TPCH_QUERY_BODY(QNR, ENABLE_RAIS, JO)                                                                          \
	virtual void Load(GrainDBBenchmarkState *state) {                                                                   \
		if (!BenchmarkRunner::TryLoadDatabase(state->db, "tpch", ENABLE_RAIS, TPCH_RAI_STMT)) {                        \
			tpch::dbgen(SF, state->db, ENABLE_RAIS);                                                                   \
			BenchmarkRunner::SaveDatabase(state->db, "tpch");                                                          \
		}                                                                                                              \
	}                                                                                                                  \
	virtual string GetQuery() {                                                                                        \
		return tpch::get_query(QNR);                                                                                   \
	}                                                                                                                  \
	virtual string GetJO() {                                                                                           \
		return tpch::get_optimized_jo(JO);                                                                             \
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

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q01, "[tpch-optimized]");
TPCH_QUERY_BODY(1, false, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q01);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q01A, "[tpch-optimized]");
TPCH_QUERY_BODY(1, true, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q01A);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q02, "[tpch-optimized]");
TPCH_QUERY_BODY(2, false, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q02);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q02A, "[tpch-optimized]");
TPCH_QUERY_BODY(2, true, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q02A);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q03, "[tpch-optimized]");
TPCH_QUERY_BODY(3, false, "Q03");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q03);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q03A, "[tpch-optimized]");
TPCH_QUERY_BODY(3, true, "Q03");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q03A);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q04, "[tpch-optimized]");
TPCH_QUERY_BODY(4, false, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q04);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q04A, "[tpch-optimized]");
TPCH_QUERY_BODY(4, true, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q04A);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q05, "[tpch-optimized]");
TPCH_QUERY_BODY(5, false, "Q05");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q05);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q05A, "[tpch-optimized]");
TPCH_QUERY_BODY(5, true, "Q05");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q05A);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q06, "[tpch-optimized]");
TPCH_QUERY_BODY(6, false, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q06);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q06A, "[tpch-optimized]");
TPCH_QUERY_BODY(6, true, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q06A);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q07, "[tpch-optimized]");
TPCH_QUERY_BODY(7, false, "Q07");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q07);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q07A, "[tpch-optimized]");
TPCH_QUERY_BODY(7, true, "Q07");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q07A);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q08, "[tpch-optimized]");
TPCH_QUERY_BODY(8, false, "Q08");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q08);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q08A, "[tpch-optimized]");
TPCH_QUERY_BODY(8, true, "Q08");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q08A);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q09, "[tpch-optimized]");
TPCH_QUERY_BODY(9, false, "Q09");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q09);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q09A, "[tpch-optimized]");
TPCH_QUERY_BODY(9, true, "Q09");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q09A);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q10, "[tpch-optimized]");
TPCH_QUERY_BODY(10, false, "Q10");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q10);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q10A, "[tpch-optimized]");
TPCH_QUERY_BODY(10, true, "Q10");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q10A);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q11, "[tpch-optimized]");
TPCH_QUERY_BODY(11, false, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q11);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q11A, "[tpch-optimized]");
TPCH_QUERY_BODY(11, true, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q11A);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q12, "[tpch-optimized]");
TPCH_QUERY_BODY(12, false, "Q12");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q12);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q12A, "[tpch-optimized]");
TPCH_QUERY_BODY(12, true, "Q12");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q12A);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q13, "[tpch-optimized]");
TPCH_QUERY_BODY(13, false, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q13);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q13A, "[tpch-optimized]");
TPCH_QUERY_BODY(13, true, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q13A);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q14, "[tpch-optimized]");
TPCH_QUERY_BODY(14, false, "Q14");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q14);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q14A, "[tpch-optimized]");
TPCH_QUERY_BODY(14, true, "Q14");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q14A);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q15, "[tpch-optimized]");
TPCH_QUERY_BODY(15, false, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q15);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q15A, "[tpch-optimized]");
TPCH_QUERY_BODY(15, true, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q15A);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q16, "[tpch-optimized]");
TPCH_QUERY_BODY(16, false, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q16);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q16A, "[tpch-optimized]");
TPCH_QUERY_BODY(16, true, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q16A);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q17, "[tpch-optimized]");
TPCH_QUERY_BODY(17, false, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q17);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q17A, "[tpch-optimized]");
TPCH_QUERY_BODY(17, true, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q17A);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q18, "[tpch-optimized]");
TPCH_QUERY_BODY(18, false, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q18);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q18A, "[tpch-optimized]");
TPCH_QUERY_BODY(18, true, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q18A);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q19, "[tpch-optimized]");
TPCH_QUERY_BODY(19, false, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q19);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q19A, "[tpch-optimized]");
TPCH_QUERY_BODY(19, true, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q19A);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q20, "[tpch-optimized]");
TPCH_QUERY_BODY(20, false, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q20);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q20A, "[tpch-optimized]");
TPCH_QUERY_BODY(20, true, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q20A);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q21, "[tpch-optimized]");
TPCH_QUERY_BODY(21, false, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q21);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q21A, "[tpch-optimized]");
TPCH_QUERY_BODY(21, true, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q21A);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q22, "[tpch-optimized]");
TPCH_QUERY_BODY(22, false, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q22);

GRAINDB_BENCHMARK(TPCH_OPTIMIZED_Q22A, "[tpch-optimized]");
TPCH_QUERY_BODY(22, true, "EMPTY");
FINISH_BENCHMARK(TPCH_OPTIMIZED_Q22A);
