#include "sqlite_benchmark.hpp"

#include "sqlite_transfer.hpp"

using namespace graindb;
using namespace std;

SQLiteBenchmark::SQLiteBenchmark(unique_ptr<GrainDBBenchmark> graindb)
    : Benchmark(true, "sqlite_" + graindb->name, "sqlite_" + graindb->group), graindb_benchmark(move(graindb)) {
}

unique_ptr<BenchmarkState> SQLiteBenchmark::Initialize() {
	auto sqlite_state = make_unique<SQLiteBenchmarkState>();
	// first load the data into GrainDB
	auto graindb_benchmark_state = graindb_benchmark->Initialize();
	auto &graindb_state = (GrainDBBenchmarkState &)*graindb_benchmark_state;
	if (sqlite3_open(":memory:", &sqlite_state->db) != SQLITE_OK) {
		return nullptr;
	}
	// then transfer the data to SQLite
	sqlite::TransferDatabase(graindb_state.conn, sqlite_state->db);
	// get the types of the query
	auto graindb_result = graindb_state.conn.Query(graindb_benchmark->GetQuery());
	if (!graindb_result->success) {
		return nullptr;
	}
	sqlite_state->types = graindb_result->sql_types;
	return move(sqlite_state);
}

void SQLiteBenchmark::Run(BenchmarkState *state_) {
	auto state = (SQLiteBenchmarkState *)state_;
	auto query = graindb_benchmark->GetQuery();
	state->result = sqlite::QueryDatabase(state->types, state->db, query, state->interrupt);
}

void SQLiteBenchmark::Cleanup(BenchmarkState *state_) {
}

string SQLiteBenchmark::Verify(BenchmarkState *state_) {
	auto state = (SQLiteBenchmarkState *)state_;
	if (!state->result) {
		return "No result!";
	}
	return graindb_benchmark->VerifyResult(state->result.get());
}

string SQLiteBenchmark::GetLogOutput(BenchmarkState *state_) {
	return "";
}

void SQLiteBenchmark::Interrupt(BenchmarkState *state_) {
	auto state = (SQLiteBenchmarkState *)state_;
	state->interrupt = 1;
}

string SQLiteBenchmark::BenchmarkInfo() {
	return graindb_benchmark->BenchmarkInfo();
}
