//===----------------------------------------------------------------------===//
//
//                         GrainDB
//
// graindb_benchmark_macro.hpp
//
// Author: Mark Raasveldt
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb_benchmark.hpp"
#include "sqlite_benchmark.hpp"

#define GRAINDB_BENCHMARK(NAME, GROUP)                                                                                  \
	class NAME##Benchmark : public GrainDBBenchmark {                                                                   \
		NAME##Benchmark(bool register_benchmark) : GrainDBBenchmark(register_benchmark, "" #NAME, GROUP) {              \
		}                                                                                                              \
                                                                                                                       \
	public:                                                                                                            \
		static NAME##Benchmark *GetInstance() {                                                                        \
			static NAME##Benchmark singleton(true);                                                                    \
			auto benchmark = unique_ptr<GrainDBBenchmark>(new NAME##Benchmark(false));                                  \
			if (!benchmark->GetQuery().empty()) {                                                                      \
				static SQLiteBenchmark sqlite_singleton(move(benchmark));                                              \
			}                                                                                                          \
			return &singleton;                                                                                         \
		}

#define REGISTER_BENCHMARK(NAME) auto global_instance_##NAME = NAME##Benchmark::GetInstance()

#define FINISH_BENCHMARK(NAME)                                                                                         \
	}                                                                                                                  \
	;                                                                                                                  \
	REGISTER_BENCHMARK(NAME);

namespace graindb {}
