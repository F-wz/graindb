#include "benchmark_runner.hpp"
#include "compare_result.hpp"
#include "graindb_benchmark_macro.hpp"
#include "ldbc.hpp"

using namespace graindb;
using namespace std;

#define SF 30

#define LDBC_QUERY_BODY(QNR, PARAM, ENABLE_RAIS, JO_NAME)                                                              \
	virtual void Load(GrainDBBenchmarkState *state) {                                                                   \
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

GRAINDB_BENCHMARK(LDBC_MICROK_Q001, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1263736391, false, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q001)

GRAINDB_BENCHMARK(LDBC_MICROK_Q001A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1263736391, true, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q001A)

GRAINDB_BENCHMARK(LDBC_MICROK_Q002, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1264096391, false, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q002)

GRAINDB_BENCHMARK(LDBC_MICROK_Q002A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1264096391, true, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q002A)

GRAINDB_BENCHMARK(LDBC_MICROK_Q003, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1264736391, false, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q003)

GRAINDB_BENCHMARK(LDBC_MICROK_Q003A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1264736391, true, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q003A)

GRAINDB_BENCHMARK(LDBC_MICROK_Q004, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1265366391, false, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q004)

GRAINDB_BENCHMARK(LDBC_MICROK_Q004A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1265366391, true, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q004A)

GRAINDB_BENCHMARK(LDBC_MICROK_Q005, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1266216391, false, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q005)

GRAINDB_BENCHMARK(LDBC_MICROK_Q005A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1266216391, true, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q005A)

GRAINDB_BENCHMARK(LDBC_MICROK_Q006, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1267916391, false, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q006)

GRAINDB_BENCHMARK(LDBC_MICROK_Q006A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1267916391, true, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q006A)

GRAINDB_BENCHMARK(LDBC_MICROK_Q007, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1270216391, false, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q007)

GRAINDB_BENCHMARK(LDBC_MICROK_Q007A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1270216391, true, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q007A)

GRAINDB_BENCHMARK(LDBC_MICROK_Q008, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1273536391, false, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q008)

GRAINDB_BENCHMARK(LDBC_MICROK_Q008A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1273536391, true, "MICROKA")
FINISH_BENCHMARK(LDBC_MICROK_Q008A)

GRAINDB_BENCHMARK(LDBC_MICROK_Q009, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1280516391, false, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q009)

GRAINDB_BENCHMARK(LDBC_MICROK_Q009A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1280516391, true, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q009A)

GRAINDB_BENCHMARK(LDBC_MICROK_Q010, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1288866391, false, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q010)

GRAINDB_BENCHMARK(LDBC_MICROK_Q010A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1288866391, true, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q010A)

GRAINDB_BENCHMARK(LDBC_MICROK_Q011, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1301606391, false, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q011)

GRAINDB_BENCHMARK(LDBC_MICROK_Q011A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1301606391, true, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q011A)

GRAINDB_BENCHMARK(LDBC_MICROK_Q012, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1311766391, false, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q012)

GRAINDB_BENCHMARK(LDBC_MICROK_Q012A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1311766391, true, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q012A)

GRAINDB_BENCHMARK(LDBC_MICROK_Q013, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1320486391, false, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q013)

GRAINDB_BENCHMARK(LDBC_MICROK_Q013A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1320486391, true, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q013A)

GRAINDB_BENCHMARK(LDBC_MICROK_Q014, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1328406391, false, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q014)

GRAINDB_BENCHMARK(LDBC_MICROK_Q014A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1328406391, true, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q014A)

GRAINDB_BENCHMARK(LDBC_MICROK_Q015, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1335216391, false, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q015)

GRAINDB_BENCHMARK(LDBC_MICROK_Q015A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1335216391, true, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q015A)

GRAINDB_BENCHMARK(LDBC_MICROK_Q016, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1341616391, false, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q016)

GRAINDB_BENCHMARK(LDBC_MICROK_Q016A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1341616391, true, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q016A)

GRAINDB_BENCHMARK(LDBC_MICROK_Q017, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1347496391, false, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q017)

GRAINDB_BENCHMARK(LDBC_MICROK_Q017A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1347496391, true, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q017A)

GRAINDB_BENCHMARK(LDBC_MICROK_Q018, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1353146391, false, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q018)

GRAINDB_BENCHMARK(LDBC_MICROK_Q018A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1353146391, true, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q018A)

GRAINDB_BENCHMARK(LDBC_MICROK_Q019, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1400000000, false, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q019)

GRAINDB_BENCHMARK(LDBC_MICROK_Q019A, "[ldbc_micro_k]")
LDBC_QUERY_BODY(1, 1400000000, true, "MICROK")
FINISH_BENCHMARK(LDBC_MICROK_Q019A)
