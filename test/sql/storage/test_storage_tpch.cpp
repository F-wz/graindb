#include "catch.hpp"
#include "graindb/common/file_system.hpp"
#include "dbgen.hpp"
#include "test_helpers.hpp"

using namespace graindb;
using namespace std;

TEST_CASE("Test storing TPC-H", "[storage][.]") {
	unique_ptr<QueryResult> result;
	double sf = 0.1;
	auto storage_database = TestCreatePath("storage_tpch");
	auto config = GetTestConfig();

	// make sure the database does not exist
	DeleteDatabase(storage_database);
	{
		// create a database and insert TPC-H tables
		GrainDB db(storage_database, config.get());
		// generate the TPC-H data for SF 0.1
		tpch::dbgen(sf, db);
	}
	// reload the database from disk
	{
		GrainDB db(storage_database, config.get());
		Connection con(db);
		// check if all the counts are correct
		result = con.Query("SELECT COUNT(*) FROM orders");
		REQUIRE(CHECK_COLUMN(result, 0, {150000}));
		result = con.Query("SELECT COUNT(*) FROM lineitem");
		REQUIRE(CHECK_COLUMN(result, 0, {600572}));
		result = con.Query("SELECT COUNT(*) FROM part");
		REQUIRE(CHECK_COLUMN(result, 0, {20000}));
		result = con.Query("SELECT COUNT(*) FROM partsupp");
		REQUIRE(CHECK_COLUMN(result, 0, {80000}));
		result = con.Query("SELECT COUNT(*) FROM supplier");
		REQUIRE(CHECK_COLUMN(result, 0, {1000}));
		result = con.Query("SELECT COUNT(*) FROM customer");
		REQUIRE(CHECK_COLUMN(result, 0, {15000}));
		result = con.Query("SELECT COUNT(*) FROM nation");
		REQUIRE(CHECK_COLUMN(result, 0, {25}));
		result = con.Query("SELECT COUNT(*) FROM region");
		REQUIRE(CHECK_COLUMN(result, 0, {5}));
	}
	// reload the database from disk again
	{
		GrainDB db(storage_database, config.get());
		Connection con(db);
		// check if all the counts are correct
		result = con.Query("SELECT COUNT(*) FROM orders");
		REQUIRE(CHECK_COLUMN(result, 0, {150000}));
		result = con.Query("SELECT COUNT(*) FROM lineitem");
		REQUIRE(CHECK_COLUMN(result, 0, {600572}));
		result = con.Query("SELECT COUNT(*) FROM part");
		REQUIRE(CHECK_COLUMN(result, 0, {20000}));
		result = con.Query("SELECT COUNT(*) FROM partsupp");
		REQUIRE(CHECK_COLUMN(result, 0, {80000}));
		result = con.Query("SELECT COUNT(*) FROM supplier");
		REQUIRE(CHECK_COLUMN(result, 0, {1000}));
		result = con.Query("SELECT COUNT(*) FROM customer");
		REQUIRE(CHECK_COLUMN(result, 0, {15000}));
		result = con.Query("SELECT COUNT(*) FROM nation");
		REQUIRE(CHECK_COLUMN(result, 0, {25}));
		result = con.Query("SELECT COUNT(*) FROM region");
		REQUIRE(CHECK_COLUMN(result, 0, {5}));
	}
	// reload the database from disk again
	{
		GrainDB db(storage_database, config.get());
		Connection con(db);
		// check if all the counts are correct
		result = con.Query("SELECT COUNT(*) FROM orders");
		REQUIRE(CHECK_COLUMN(result, 0, {150000}));
		result = con.Query("SELECT COUNT(*) FROM lineitem");
		REQUIRE(CHECK_COLUMN(result, 0, {600572}));
		result = con.Query("SELECT COUNT(*) FROM part");
		REQUIRE(CHECK_COLUMN(result, 0, {20000}));
		result = con.Query("SELECT COUNT(*) FROM partsupp");
		REQUIRE(CHECK_COLUMN(result, 0, {80000}));
		result = con.Query("SELECT COUNT(*) FROM supplier");
		REQUIRE(CHECK_COLUMN(result, 0, {1000}));
		result = con.Query("SELECT COUNT(*) FROM customer");
		REQUIRE(CHECK_COLUMN(result, 0, {15000}));
		result = con.Query("SELECT COUNT(*) FROM nation");
		REQUIRE(CHECK_COLUMN(result, 0, {25}));
		result = con.Query("SELECT COUNT(*) FROM region");
		REQUIRE(CHECK_COLUMN(result, 0, {5}));
	}
	DeleteDatabase(storage_database);
}
