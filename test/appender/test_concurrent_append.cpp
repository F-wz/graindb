#include "catch.hpp"
#include "graindb/main/appender.hpp"
#include "test_helpers.hpp"

#include <atomic>
#include <thread>
#include <vector>

using namespace graindb;
using namespace std;

atomic<int> finished_threads;

#define THREAD_COUNT 20
#define INSERT_ELEMENTS 2000

static void append_to_integers(GrainDB *db, size_t threadnr) {
	REQUIRE(db);
	Connection con(*db);

	Appender appender(con, "integers");
	for (size_t i = 0; i < INSERT_ELEMENTS; i++) {
		appender.BeginRow();
		appender.Append<int32_t>(1);
		appender.EndRow();
	}
	finished_threads++;
	while (finished_threads != THREAD_COUNT)
		;
	appender.Close();
}

TEST_CASE("Test concurrent appends", "[appender][.]") {
	unique_ptr<QueryResult> result;
	GrainDB db(nullptr);
	Connection con(db);

	// create a single table to append to
	REQUIRE_NO_FAIL(con.Query("CREATE TABLE integers(i INTEGER)"));

	finished_threads = 0;

	thread threads[THREAD_COUNT];
	for (size_t i = 0; i < THREAD_COUNT; i++) {
		threads[i] = thread(append_to_integers, &db, i);
	}

	for (size_t i = 0; i < THREAD_COUNT; i++) {
		threads[i].join();
	}
	// check how many entries we have
	result = con.Query("SELECT COUNT(*) FROM integers");
	REQUIRE(CHECK_COLUMN(result, 0, {THREAD_COUNT * INSERT_ELEMENTS}));
}
