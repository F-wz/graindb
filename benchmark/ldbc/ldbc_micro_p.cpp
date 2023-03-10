#include "benchmark_runner.hpp"
#include "compare_result.hpp"
#include "graindb_benchmark_macro.hpp"
#include "ldbc.hpp"

using namespace graindb;
using namespace std;

#define SF 10

#define LDBC_QUERY_BODY(QNR, PARAM, ENABLE_RAIS, JO_NAME)                                                              \
	virtual void Load(GrainDBBenchmarkState *state) {                                                                   \
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

GRAINDB_BENCHMARK(LDBC_MICROP_Q001, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 20, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q001)

GRAINDB_BENCHMARK(LDBC_MICROP_Q001A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 20, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q001A)

GRAINDB_BENCHMARK(LDBC_MICROP_Q002, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 40, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q002)

GRAINDB_BENCHMARK(LDBC_MICROP_Q002A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 40, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q002A)

GRAINDB_BENCHMARK(LDBC_MICROP_Q003, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 80, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q003)

GRAINDB_BENCHMARK(LDBC_MICROP_Q003A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 80, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q003A)

GRAINDB_BENCHMARK(LDBC_MICROP_Q004, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 150, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q004)

GRAINDB_BENCHMARK(LDBC_MICROP_Q004A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 150, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q004A)

GRAINDB_BENCHMARK(LDBC_MICROP_Q005, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 270, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q005)

GRAINDB_BENCHMARK(LDBC_MICROP_Q005A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 270, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q005A)

GRAINDB_BENCHMARK(LDBC_MICROP_Q006, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 640, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q006)

GRAINDB_BENCHMARK(LDBC_MICROP_Q006A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 640, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q006A)

GRAINDB_BENCHMARK(LDBC_MICROP_Q007, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 2000, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q007)

GRAINDB_BENCHMARK(LDBC_MICROP_Q007A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 2000, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q007A)

GRAINDB_BENCHMARK(LDBC_MICROP_Q008, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 3800, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q008)

GRAINDB_BENCHMARK(LDBC_MICROP_Q008A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 3800, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q008A)

GRAINDB_BENCHMARK(LDBC_MICROP_Q009, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 9300, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q009)

GRAINDB_BENCHMARK(LDBC_MICROP_Q009A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 9300, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q009A)

GRAINDB_BENCHMARK(LDBC_MICROP_Q010, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 18700, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q010)

GRAINDB_BENCHMARK(LDBC_MICROP_Q010A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 18700, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q010A)

GRAINDB_BENCHMARK(LDBC_MICROP_Q011, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 37600, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q011)

GRAINDB_BENCHMARK(LDBC_MICROP_Q011A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 37600, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q011A)

GRAINDB_BENCHMARK(LDBC_MICROP_Q012, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 55700, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q012)

GRAINDB_BENCHMARK(LDBC_MICROP_Q012A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 55700, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q012A)

GRAINDB_BENCHMARK(LDBC_MICROP_Q013, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 74200, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q013)

GRAINDB_BENCHMARK(LDBC_MICROP_Q013A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 74200, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q013A)

GRAINDB_BENCHMARK(LDBC_MICROP_Q014, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 92700, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q014)

GRAINDB_BENCHMARK(LDBC_MICROP_Q014A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 92700, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q014A)

GRAINDB_BENCHMARK(LDBC_MICROP_Q015, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 110800, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q015)

GRAINDB_BENCHMARK(LDBC_MICROP_Q015A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 110800, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q015A)

GRAINDB_BENCHMARK(LDBC_MICROP_Q016, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 129100, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q016)

GRAINDB_BENCHMARK(LDBC_MICROP_Q016A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 129100, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q016A)

GRAINDB_BENCHMARK(LDBC_MICROP_Q017, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 147300, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q017)

GRAINDB_BENCHMARK(LDBC_MICROP_Q017A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 147300, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q017A)

GRAINDB_BENCHMARK(LDBC_MICROP_Q018, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 165500, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q018)

GRAINDB_BENCHMARK(LDBC_MICROP_Q018A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 165500, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q018A)

GRAINDB_BENCHMARK(LDBC_MICROP_Q019, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 184000, false, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q019)

GRAINDB_BENCHMARK(LDBC_MICROP_Q019A, "[ldbc_micro_p]")
LDBC_QUERY_BODY(2, 184000, true, "MICROP")
FINISH_BENCHMARK(LDBC_MICROP_Q019A)
