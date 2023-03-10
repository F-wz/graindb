//===----------------------------------------------------------------------===//
//
//                         GrainDB
//
// graindb_benchmark.hpp
//
// Author: Mark Raasveldt
//
//===----------------------------------------------------------------------===//

#pragma once

#include "benchmark.hpp"
#include "graindb.hpp"
#include "graindb/main/client_context.hpp"
#include "test_helpers.hpp"

namespace graindb {

//! Base class for any state that has to be kept by a Benchmark
struct GrainDBBenchmarkState : public BenchmarkState {
	GrainDB db;
	Connection conn;

	explicit GrainDBBenchmarkState(string path) : db(path.empty() ? nullptr : path.c_str()), conn(db) {
		conn.DisableProfiling();
	}
	virtual ~GrainDBBenchmarkState() {
	}

	void EnableProfiling() override {
		conn.EnableProfiling();
	}
	void DisableProfiling() override {
		conn.DisableProfiling();
	}
};

//! The base Benchmark class is a base class that is used to create and register
//! new benchmarks
class GrainDBBenchmark : public Benchmark {
public:
	GrainDBBenchmark(bool register_benchmark, string name, string group) : Benchmark(register_benchmark, name, group) {
	}
	virtual ~GrainDBBenchmark() {
	}

	//! Load data into GrainDB
	virtual void Load(GrainDBBenchmarkState *state) = 0;
	//! A single query to run against the database
	virtual string GetQuery() {
		return string();
	}
	virtual string GetJO() {
		return string();
	}
	virtual string GetCard() {
		return string();
	}
	virtual string GetViews() {
		return string();
	}

	//! Run a bunch of queries, only called if GetQuery() returns an empty string
	virtual void RunBenchmark(GrainDBBenchmarkState *state) {
	}
	//! This function gets called after the GetQuery() method
	virtual void Cleanup(GrainDBBenchmarkState *state){};
	//! Verify a result
	virtual string VerifyResult(QueryResult *result) = 0;
	//! Whether or not the benchmark is performed on an in-memory database
	virtual bool InMemory() {
		return true;
	}

	string GetDatabasePath() {
		if (!InMemory()) {
			string path = "graindb_benchmark_db.db";
			DeleteDatabase(path);
			return path;
		} else {
			return string();
		}
	}

	virtual unique_ptr<GrainDBBenchmarkState> CreateBenchmarkState() {
		return make_unique<GrainDBBenchmarkState>(GetDatabasePath());
	}

	unique_ptr<BenchmarkState> Initialize() override {
		auto state = CreateBenchmarkState();
		Load(state.get());
		string views = GetViews();
		if (!views.empty()) {
			state->conn.Query(views);
		}
		string jo = GetJO();
		if (!jo.empty()) {
			state->conn.EnableExplicitJoinOrder(jo);
		}
		string card = GetCard();
		if (!card.empty()) {
			state->conn.EnableExplicitCardinality(card);
		}
		return move(state);
	}

	void Run(BenchmarkState *state_) override {
		auto state = (GrainDBBenchmarkState *)state_;
		string query = GetQuery();
		if (query.empty()) {
			RunBenchmark(state);
		} else {
			state->result = state->conn.Query(query);
		}
	}

	void Cleanup(BenchmarkState *state_) override {
		auto state = (GrainDBBenchmarkState *)state_;
		Cleanup(state);
	}

	string Verify(BenchmarkState *state_) override {
		auto state = (GrainDBBenchmarkState *)state_;
		return VerifyResult(state->result.get());
	}

	string GetLogOutput(BenchmarkState *state_) override {
		auto state = (GrainDBBenchmarkState *)state_;
		return state->conn.context->profiler.ToJSON();
	}

	//! Interrupt the benchmark because of a timeout
	void Interrupt(BenchmarkState *state_) override {
		auto state = (GrainDBBenchmarkState *)state_;
		state->conn.Interrupt();
	}
};

} // namespace graindb
