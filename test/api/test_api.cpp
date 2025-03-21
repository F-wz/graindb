#include "catch.hpp"
#include "test_helpers.hpp"

#include <chrono>
#include <thread>

using namespace graindb;
using namespace std;

TEST_CASE("Test using connection after database is gone", "[api]") {
	auto db = make_unique<GrainDB>(nullptr);
	auto conn = make_unique<Connection>(*db);
	// check that the connection works
	auto result = conn->Query("SELECT 42");
	REQUIRE(CHECK_COLUMN(result, 0, {42}));
	// destroy the database
	db.reset();
	// try to use the connection
	REQUIRE_FAIL(conn->Query("SELECT 42"));

	// now try it with an open transaction
	db = make_unique<GrainDB>(nullptr);
	conn = make_unique<Connection>(*db);

	REQUIRE_NO_FAIL(conn->Query("BEGIN TRANSACTION"));
	result = conn->Query("SELECT 42");
	REQUIRE(CHECK_COLUMN(result, 0, {42}));

	db.reset();

	REQUIRE_FAIL(conn->Query("SELECT 42"));
}

TEST_CASE("Test destroying connections with open transactions", "[api]") {
	auto db = make_unique<GrainDB>(nullptr);
	{
		Connection con(*db);
		con.Query("BEGIN TRANSACTION");
		con.Query("CREATE TABLE test(i INTEGER);");
	}

	auto conn = make_unique<Connection>(*db);
	REQUIRE_NO_FAIL(conn->Query("CREATE TABLE test(i INTEGER)"));
}

static void long_running_query(Connection *conn, bool *correct) {
	*correct = true;
	auto result = conn->Query("SELECT i1.i FROM integers i1, integers i2, integers i3, integers i4, integers i5, "
	                          "integers i6, integers i7, integers i8, integers i9, integers i10,"
	                          "integers i11, integers i12, integers i13");
	// the query should fail
	*correct = !result->success;
}

TEST_CASE("Test closing database during long running query", "[api]") {
	auto db = make_unique<GrainDB>(nullptr);
	auto conn = make_unique<Connection>(*db);
	// create the database
	REQUIRE_NO_FAIL(conn->Query("CREATE TABLE integers(i INTEGER)"));
	REQUIRE_NO_FAIL(conn->Query("INSERT INTO integers VALUES (1), (2), (3), (NULL)"));
	conn->DisableProfiling();
	// perform a long running query in the background (many cross products)
	bool correct;
	auto background_thread = thread(long_running_query, conn.get(), &correct);
	// wait a little bit
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	// destroy the database
	db.reset();
	// wait for the thread
	background_thread.join();
	REQUIRE(correct);
	// try to use the connection
	REQUIRE_FAIL(conn->Query("SELECT 42"));
}

TEST_CASE("Test closing result after database is gone", "[api]") {
	auto db = make_unique<GrainDB>(nullptr);
	auto conn = make_unique<Connection>(*db);
	// check that the connection works
	auto result = conn->Query("SELECT 42");
	REQUIRE(CHECK_COLUMN(result, 0, {42}));
	// destroy the database
	db.reset();
	conn.reset();
	result.reset();

	// now the streaming result
	db = make_unique<GrainDB>(nullptr);
	conn = make_unique<Connection>(*db);
	// check that the connection works
	auto streaming_result = conn->SendQuery("SELECT 42");
	// destroy the database
	db.reset();
	conn.reset();
	streaming_result.reset();
}

TEST_CASE("Test closing database with open prepared statements", "[api]") {
	auto db = make_unique<GrainDB>(nullptr);
	auto conn = make_unique<Connection>(*db);

	auto p1 = conn->Prepare("CREATE TABLE a (i INTEGER)");
	p1->Execute();
	auto p2 = conn->Prepare("INSERT INTO a VALUES (42)");
	p2->Execute();

	db.reset();
	conn.reset();
}

static void parallel_query(Connection *conn, bool *correct, size_t threadnr) {
	correct[threadnr] = true;
	for (size_t i = 0; i < 100; i++) {
		auto result = conn->Query("SELECT * FROM integers ORDER BY i");
		if (!CHECK_COLUMN(result, 0, {Value(), 1, 2, 3})) {
			correct[threadnr] = false;
		}
	}
}

TEST_CASE("Test parallel usage of single client", "[api][.]") {
	auto db = make_unique<GrainDB>(nullptr);
	auto conn = make_unique<Connection>(*db);

	REQUIRE_NO_FAIL(conn->Query("CREATE TABLE integers(i INTEGER)"));
	REQUIRE_NO_FAIL(conn->Query("INSERT INTO integers VALUES (1), (2), (3), (NULL)"));

	bool correct[20];
	thread threads[20];
	for (size_t i = 0; i < 20; i++) {
		threads[i] = thread(parallel_query, conn.get(), correct, i);
	}
	for (size_t i = 0; i < 20; i++) {
		threads[i].join();
		REQUIRE(correct[i]);
	}
}

static void parallel_query_with_new_connection(GrainDB *db, bool *correct, size_t threadnr) {
	correct[threadnr] = true;
	for (size_t i = 0; i < 100; i++) {
		auto conn = make_unique<Connection>(*db);
		auto result = conn->Query("SELECT * FROM integers ORDER BY i");
		if (!CHECK_COLUMN(result, 0, {Value(), 1, 2, 3})) {
			correct[threadnr] = false;
		}
	}
}

TEST_CASE("Test making and dropping connections in parallel to a single database", "[api][.]") {
	auto db = make_unique<GrainDB>(nullptr);
	auto conn = make_unique<Connection>(*db);

	REQUIRE_NO_FAIL(conn->Query("CREATE TABLE integers(i INTEGER)"));
	REQUIRE_NO_FAIL(conn->Query("INSERT INTO integers VALUES (1), (2), (3), (NULL)"));

	bool correct[20];
	thread threads[20];
	for (size_t i = 0; i < 20; i++) {
		threads[i] = thread(parallel_query_with_new_connection, db.get(), correct, i);
	}
	for (size_t i = 0; i < 100; i++) {
		auto result = conn->Query("SELECT * FROM integers ORDER BY i");
		REQUIRE(CHECK_COLUMN(result, 0, {Value(), 1, 2, 3}));
	}
	for (size_t i = 0; i < 20; i++) {
		threads[i].join();
		REQUIRE(correct[i]);
	}
	auto result = conn->Query("SELECT * FROM integers ORDER BY i");
	REQUIRE(CHECK_COLUMN(result, 0, {Value(), 1, 2, 3}));
}

TEST_CASE("Test multiple result sets", "[api]") {
	unique_ptr<QueryResult> result;
	GrainDB db(nullptr);
	Connection con(db);
	con.EnableQueryVerification();

	result = con.Query("SELECT 42; SELECT 84");
	REQUIRE(CHECK_COLUMN(result, 0, {42}));
	result = move(result->next);
	REQUIRE(CHECK_COLUMN(result, 0, {84}));
	REQUIRE(!result->next);

	// also with stream api
	result = con.SendQuery("SELECT 42; SELECT 84");
	REQUIRE(CHECK_COLUMN(result, 0, {42}));
	result = move(result->next);
	REQUIRE(CHECK_COLUMN(result, 0, {84}));
	REQUIRE(!result->next);
}

TEST_CASE("Test fetch API", "[api]") {
	GrainDB db(nullptr);
	Connection con(db);
	con.EnableQueryVerification();

	unique_ptr<QueryResult> result;

	result = con.SendQuery("CREATE TABLE test (a INTEGER);");

	result = con.Query("select a from test where 1 <> 1");
	REQUIRE(CHECK_COLUMN(result, 0, {}));

	result = con.SendQuery("INSERT INTO test VALUES (42)");
	result = con.SendQuery("SELECT a from test");
	REQUIRE(CHECK_COLUMN(result, 0, {42}));

	auto materialized_result = con.Query("select a from test");
	REQUIRE(CHECK_COLUMN(materialized_result, 0, {42}));

	// override fetch result
	result = con.SendQuery("SELECT a from test");
	result = con.SendQuery("SELECT a from test");
	result = con.SendQuery("SELECT a from test");
	result = con.SendQuery("SELECT a from test");
	REQUIRE(CHECK_COLUMN(result, 0, {42}));
}

TEST_CASE("Test fetch API robustness", "[api]") {
	auto db = make_unique<GrainDB>(nullptr);
	auto conn = make_unique<Connection>(*db);

	// remove connection with active stream result
	auto result = conn->SendQuery("SELECT 42");
	// close the connection
	conn.reset();
	// now try to fetch a chunk, this should return a nullptr
	auto chunk = result->Fetch();
	REQUIRE(!chunk);

	// now close the entire database
	conn = make_unique<Connection>(*db);
	result = conn->SendQuery("SELECT 42");

	db.reset();
	// fetch should fail
	chunk = result->Fetch();
	REQUIRE(!chunk);
	// new queries on the connection should fail as well
	REQUIRE_FAIL(conn->SendQuery("SELECT 42"));

	// override fetch result
	db = make_unique<GrainDB>(nullptr);
	conn = make_unique<Connection>(*db);
	auto result1 = conn->SendQuery("SELECT 42");
	auto result2 = conn->SendQuery("SELECT 84");
	REQUIRE_NO_FAIL(*result1);
	REQUIRE_NO_FAIL(*result2);

	// result1 should be closed now
	REQUIRE(!result1->Fetch());
	// result2 should work
	REQUIRE(result2->Fetch());

	// test materialize
	result1 = conn->SendQuery("SELECT 42");
	REQUIRE(result1->type == QueryResultType::STREAM_RESULT);
	auto materialized = ((StreamQueryResult &)*result1).Materialize();
	result2 = conn->SendQuery("SELECT 84");

	// we can read materialized still, even after opening a new result
	REQUIRE(CHECK_COLUMN(materialized, 0, {42}));
	REQUIRE(CHECK_COLUMN(result2, 0, {84}));
}

static void VerifyStreamResult(unique_ptr<QueryResult> result) {
	REQUIRE(result->types[0] == TypeId::INT32);
	size_t current_row = 0;
	size_t current_expected_value = 0;
	size_t expected_rows = 500 * 5;
	while (true) {
		auto chunk = result->Fetch();
		if (chunk->size() == 0) {
			break;
		}
		auto col1_data = FlatVector::GetData<int>(chunk->data[0]);
		for (size_t k = 0; k < chunk->size(); k++) {
			if (current_row % 500 == 0) {
				current_expected_value++;
			}
			REQUIRE(col1_data[k] == current_expected_value);
			current_row++;
		}
	}
	REQUIRE(current_row == expected_rows);
}

TEST_CASE("Test fetch API with big results", "[api][.]") {
	GrainDB db(nullptr);
	Connection con(db);
	con.EnableQueryVerification();

	// create table that consists of multiple chunks
	REQUIRE_NO_FAIL(con.Query("BEGIN TRANSACTION"));
	REQUIRE_NO_FAIL(con.Query("CREATE TABLE test(a INTEGER)"));
	for (size_t i = 0; i < 500; i++) {
		REQUIRE_NO_FAIL(con.Query("INSERT INTO test VALUES (1); INSERT INTO test VALUES (2); INSERT INTO test VALUES "
		                          "(3); INSERT INTO test VALUES (4); INSERT INTO test VALUES (5);"));
	}
	REQUIRE_NO_FAIL(con.Query("COMMIT"));

	// stream the results using the Fetch() API
	auto result = con.SendQuery("SELECT CAST(a AS INTEGER) FROM test ORDER BY a");
	VerifyStreamResult(move(result));
	// we can also stream a materialized result
	auto materialized = con.Query("SELECT CAST(a AS INTEGER) FROM test ORDER BY a");
	VerifyStreamResult(move(materialized));
	// return multiple results using the stream API
	result = con.SendQuery("SELECT CAST(a AS INTEGER) FROM test ORDER BY a; SELECT CAST(a AS INTEGER) FROM test ORDER "
	                       "BY a; SELECT CAST(a AS INTEGER) FROM test ORDER BY a;");
	auto next = move(result->next);
	while (next) {
		auto nextnext = move(next->next);
		VerifyStreamResult(move(nextnext));
		next = move(nextnext);
	}
	VerifyStreamResult(move(result));
}
