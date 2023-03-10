//===----------------------------------------------------------------------===//
//
//                         GrainDB
//
// benchmark_runner.hpp
//
// Author: Mark Raasveldt
//
//===----------------------------------------------------------------------===//

#pragma once

#include "benchmark.hpp"
#include "graindb/common/constants.hpp"
#include "graindb/common/fstream.hpp"
#include "graindb/main/connection.hpp"

namespace graindb {
class GrainDB;

//! The benchmark runner class is responsible for running benchmarks
class BenchmarkRunner {
	BenchmarkRunner() {
	}

public:
	static constexpr const char *GRAINDB_BENCHMARK_DIRECTORY = "graindb_benchmark_data";

	static BenchmarkRunner &GetInstance() {
		static BenchmarkRunner instance;
		return instance;
	}

	//! Save the current database state, exporting it to a set of CSVs in the GRAINDB_BENCHMARK_DIRECTORY directory
	static void SaveDatabase(GrainDB &db, string name);
	//! Try to initialize the database from the GRAINDB_BENCHMARK_DIRECTORY
	static bool TryLoadDatabase(GrainDB &db, string name, bool enableRAIs = false, string rai_stmt = "");
	//! Inject join order into optimizer
	static void InjectJO(Connection &conn, string jo_path);

	//! Register a benchmark in the Benchmark Runner, this is done automatically
	//! as long as the proper macro's are used
	static void RegisterBenchmark(Benchmark *benchmark);

	void Log(string message);
	void LogLine(string message);
	void LogResult(string message);
	void LogOutput(string message);

	void RunBenchmark(Benchmark *benchmark);
	void RunBenchmarks();

	vector<Benchmark *> benchmarks;
	ofstream out_file;
	ofstream log_file;
};

} // namespace graindb
