#include "catch.hpp"
#include "graindb/common/file_system.hpp"
#include "test_helpers.hpp"

using namespace graindb;
using namespace std;

TEST_CASE("Create and drop a schema+table over different runs", "[storage]") {
	unique_ptr<QueryResult> result;
	auto storage_database = TestCreatePath("storage_test");
	auto config = GetTestConfig();

	// make sure the database does not exist
	DeleteDatabase(storage_database);
	{
		// create a database and insert values
		GrainDB db(storage_database, config.get());
		Connection con(db);
		REQUIRE_NO_FAIL(con.Query("CREATE SCHEMA test;"));
		REQUIRE_NO_FAIL(con.Query("CREATE TABLE test.test (a INTEGER, b INTEGER);"));
		REQUIRE_NO_FAIL(con.Query("INSERT INTO test.test VALUES (11, 22), (13, 22);"));
		REQUIRE_NO_FAIL(con.Query("DROP TABLE test.test"));
		REQUIRE_NO_FAIL(con.Query("DROP SCHEMA test"));

		REQUIRE_NO_FAIL(con.Query("CREATE SCHEMA test;"));
		REQUIRE_NO_FAIL(con.Query("CREATE TABLE test.test (a INTEGER, b INTEGER);"));
		REQUIRE_NO_FAIL(con.Query("INSERT INTO test.test VALUES (11, 22), (13, 22);"));
	}
	// reload the database from disk
	{
		GrainDB db(storage_database, config.get());
		Connection con(db);
		REQUIRE_NO_FAIL(con.Query("DROP TABLE test.test"));
		REQUIRE_NO_FAIL(con.Query("DROP SCHEMA test"));
	}
	DeleteDatabase(storage_database);
}
