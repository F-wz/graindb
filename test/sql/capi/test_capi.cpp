#include "catch.hpp"
#include "graindb.h"
#include "test_helpers.hpp"
#include "graindb/common/exception.hpp"

using namespace graindb;
using namespace std;

class CAPIResult {
public:
	~CAPIResult() {
		graindb_destroy_result(&result);
	}
	void Query(graindb_connection connection, string query) {
		success = (graindb_query(connection, query.c_str(), &result) == GrainDBSuccess);
	}

	idx_t column_count() {
		return result.column_count;
	}

	idx_t row_count() {
		return result.row_count;
	}

	template <class T> T Fetch(idx_t col, idx_t row) {
		throw NotImplementedException("Unimplemented type for fetch");
	}

	bool IsNull(idx_t col, idx_t row) {
		return result.columns[col].nullmask[row];
	}

public:
	bool success = false;

private:
	graindb_result result;
};

static bool NO_FAIL(CAPIResult &result) {
	return result.success;
}

static bool NO_FAIL(unique_ptr<CAPIResult> result) {
	return NO_FAIL(*result);
}

template <> bool CAPIResult::Fetch(idx_t col, idx_t row) {
	return graindb_value_boolean(&result, col, row);
}

template <> int8_t CAPIResult::Fetch(idx_t col, idx_t row) {
	return graindb_value_int8(&result, col, row);
}

template <> int16_t CAPIResult::Fetch(idx_t col, idx_t row) {
	return graindb_value_int16(&result, col, row);
}

template <> int32_t CAPIResult::Fetch(idx_t col, idx_t row) {
	return graindb_value_int32(&result, col, row);
}

template <> int64_t CAPIResult::Fetch(idx_t col, idx_t row) {
	return graindb_value_int64(&result, col, row);
}

template <> float CAPIResult::Fetch(idx_t col, idx_t row) {
	return graindb_value_float(&result, col, row);
}

template <> double CAPIResult::Fetch(idx_t col, idx_t row) {
	return graindb_value_double(&result, col, row);
}

template <> graindb_date CAPIResult::Fetch(idx_t col, idx_t row) {
	auto data = (graindb_date *)result.columns[col].data;
	return data[row];
}

template <> graindb_timestamp CAPIResult::Fetch(idx_t col, idx_t row) {
	auto data = (graindb_timestamp *)result.columns[col].data;
	return data[row];
}

template <> string CAPIResult::Fetch(idx_t col, idx_t row) {
	auto value = graindb_value_varchar(&result, col, row);
	string strval = string(value);
	free((void *)value);
	return strval;
}

class CAPITester {
public:
	CAPITester() : database(nullptr), connection(nullptr) {
	}
	~CAPITester() {
		Cleanup();
	}

	void Cleanup() {
		if (connection) {
			graindb_disconnect(&connection);
			connection = nullptr;
		}
		if (database) {
			graindb_close(&database);
			database = nullptr;
		}
	}

	bool OpenDatabase(const char *path) {
		Cleanup();
		if (graindb_open(path, &database) != GrainDBSuccess) {
			return false;
		}
		if (graindb_connect(database, &connection) != GrainDBSuccess) {
			return false;
		}
		return true;
	}

	unique_ptr<CAPIResult> Query(string query) {
		auto result = make_unique<CAPIResult>();
		result->Query(connection, query);
		return result;
	}

	graindb_database database = nullptr;
	graindb_connection connection = nullptr;
};

TEST_CASE("Basic test of C API", "[capi]") {
	CAPITester tester;
	unique_ptr<CAPIResult> result;

	// open the database in in-memory mode
	REQUIRE(tester.OpenDatabase(nullptr));

	// select scalar value
	result = tester.Query("SELECT CAST(42 AS BIGINT)");
	REQUIRE_NO_FAIL(*result);
	REQUIRE(result->column_count() == 1);
	REQUIRE(result->row_count() == 1);
	REQUIRE(result->Fetch<int64_t>(0, 0) == 42);
	REQUIRE(!result->IsNull(0, 0));

	// select scalar NULL
	result = tester.Query("SELECT NULL");
	REQUIRE_NO_FAIL(*result);
	REQUIRE(result->column_count() == 1);
	REQUIRE(result->row_count() == 1);
	REQUIRE(result->Fetch<int64_t>(0, 0) == 0);
	REQUIRE(result->IsNull(0, 0));

	// select scalar string
	result = tester.Query("SELECT 'hello'");
	REQUIRE_NO_FAIL(*result);
	REQUIRE(result->column_count() == 1);
	REQUIRE(result->row_count() == 1);
	REQUIRE(result->Fetch<string>(0, 0) == "hello");
	REQUIRE(!result->IsNull(0, 0));

	// multiple insertions
	REQUIRE_NO_FAIL(tester.Query("CREATE TABLE test (a INTEGER, b INTEGER);"));
	REQUIRE_NO_FAIL(tester.Query("INSERT INTO test VALUES (11, 22)"));
	REQUIRE_NO_FAIL(tester.Query("INSERT INTO test VALUES (NULL, 21)"));
	REQUIRE_NO_FAIL(tester.Query("INSERT INTO test VALUES (13, 22)"));

	// NULL selection
	result = tester.Query("SELECT a, b FROM test ORDER BY a");
	REQUIRE_NO_FAIL(*result);
	// NULL, 11, 13
	REQUIRE(result->IsNull(0, 0));
	REQUIRE(result->Fetch<int32_t>(0, 1) == 11);
	REQUIRE(result->Fetch<int32_t>(0, 2) == 13);
	// 21, 22, 22
	REQUIRE(result->Fetch<int32_t>(1, 0) == 21);
	REQUIRE(result->Fetch<int32_t>(1, 1) == 22);
	REQUIRE(result->Fetch<int32_t>(1, 2) == 22);
}

TEST_CASE("Test different types of C API", "[capi]") {
	CAPITester tester;
	unique_ptr<CAPIResult> result;

	// open the database in in-memory mode
	REQUIRE(tester.OpenDatabase(nullptr));

	// integer columns
	vector<string> types = {"TINYINT", "SMALLINT", "INTEGER", "BIGINT"};
	for (auto &type : types) {
		// create the table and insert values
		REQUIRE_NO_FAIL(tester.Query("BEGIN TRANSACTION"));
		REQUIRE_NO_FAIL(tester.Query("CREATE TABLE integers(i " + type + ")"));
		REQUIRE_NO_FAIL(tester.Query("INSERT INTO integers VALUES (1), (NULL)"));

		result = tester.Query("SELECT * FROM integers ORDER BY i");
		REQUIRE_NO_FAIL(*result);
		REQUIRE(result->IsNull(0, 0));
		REQUIRE(result->Fetch<int8_t>(0, 0) == 0);
		REQUIRE(result->Fetch<int16_t>(0, 0) == 0);
		REQUIRE(result->Fetch<int32_t>(0, 0) == 0);
		REQUIRE(result->Fetch<int64_t>(0, 0) == 0);
		REQUIRE(ApproxEqual(result->Fetch<float>(0, 0), 0.0f));
		REQUIRE(ApproxEqual(result->Fetch<double>(0, 0), 0.0));

		REQUIRE(!result->IsNull(0, 1));
		REQUIRE(result->Fetch<int8_t>(0, 1) == 1);
		REQUIRE(result->Fetch<int16_t>(0, 1) == 1);
		REQUIRE(result->Fetch<int32_t>(0, 1) == 1);
		REQUIRE(result->Fetch<int64_t>(0, 1) == 1);
		REQUIRE(ApproxEqual(result->Fetch<float>(0, 1), 1.0f));
		REQUIRE(ApproxEqual(result->Fetch<double>(0, 1), 1.0));
		REQUIRE(result->Fetch<string>(0, 1) == "1");

		REQUIRE_NO_FAIL(tester.Query("ROLLBACK"));
	}
	// real/double columns
	types = {"REAL", "DOUBLE"};
	for (auto &type : types) {
		// create the table and insert values
		REQUIRE_NO_FAIL(tester.Query("BEGIN TRANSACTION"));
		REQUIRE_NO_FAIL(tester.Query("CREATE TABLE doubles(i " + type + ")"));
		REQUIRE_NO_FAIL(tester.Query("INSERT INTO doubles VALUES (1), (NULL)"));

		result = tester.Query("SELECT * FROM doubles ORDER BY i");
		REQUIRE_NO_FAIL(*result);
		REQUIRE(result->IsNull(0, 0));
		REQUIRE(result->Fetch<int8_t>(0, 0) == 0);
		REQUIRE(result->Fetch<int16_t>(0, 0) == 0);
		REQUIRE(result->Fetch<int32_t>(0, 0) == 0);
		REQUIRE(result->Fetch<int64_t>(0, 0) == 0);
		REQUIRE(ApproxEqual(result->Fetch<float>(0, 0), 0.0f));
		REQUIRE(ApproxEqual(result->Fetch<double>(0, 0), 0.0));

		REQUIRE(!result->IsNull(0, 1));
		REQUIRE(result->Fetch<int8_t>(0, 1) == 1);
		REQUIRE(result->Fetch<int16_t>(0, 1) == 1);
		REQUIRE(result->Fetch<int32_t>(0, 1) == 1);
		REQUIRE(result->Fetch<int64_t>(0, 1) == 1);
		REQUIRE(ApproxEqual(result->Fetch<float>(0, 1), 1.0f));
		REQUIRE(ApproxEqual(result->Fetch<double>(0, 1), 1.0));

		REQUIRE_NO_FAIL(tester.Query("ROLLBACK"));
	}
	// date columns
	REQUIRE_NO_FAIL(tester.Query("CREATE TABLE dates(d DATE)"));
	REQUIRE_NO_FAIL(tester.Query("INSERT INTO dates VALUES ('1992-09-20'), (NULL), ('1000000-09-20')"));

	result = tester.Query("SELECT * FROM dates ORDER BY d");
	REQUIRE_NO_FAIL(*result);
	REQUIRE(result->IsNull(0, 0));
	graindb_date date = result->Fetch<graindb_date>(0, 1);
	REQUIRE(date.year == 1992);
	REQUIRE(date.month == 9);
	REQUIRE(date.day == 20);
	REQUIRE(result->Fetch<string>(0, 1) == Value::DATE(1992, 9, 20).ToString(SQLType::DATE));
	date = result->Fetch<graindb_date>(0, 2);
	REQUIRE(date.year == 1000000);
	REQUIRE(date.month == 9);
	REQUIRE(date.day == 20);
	REQUIRE(result->Fetch<string>(0, 2) == Value::DATE(1000000, 9, 20).ToString(SQLType::DATE));

	// timestamp columns
	REQUIRE_NO_FAIL(tester.Query("CREATE TABLE timestamps(t TIMESTAMP)"));
	REQUIRE_NO_FAIL(tester.Query("INSERT INTO timestamps VALUES ('1992-09-20 12:01:30'), (NULL)"));

	result = tester.Query("SELECT * FROM timestamps ORDER BY t");
	REQUIRE_NO_FAIL(*result);
	REQUIRE(result->IsNull(0, 0));
	graindb_timestamp stamp = result->Fetch<graindb_timestamp>(0, 1);
	REQUIRE(stamp.date.year == 1992);
	REQUIRE(stamp.date.month == 9);
	REQUIRE(stamp.date.day == 20);
	REQUIRE(stamp.time.hour == 12);
	REQUIRE(stamp.time.min == 1);
	REQUIRE(stamp.time.sec == 30);
	REQUIRE(stamp.time.msec == 0);
	REQUIRE(result->Fetch<string>(0, 1) == Value::TIMESTAMP(1992, 9, 20, 12, 1, 30, 0).ToString(SQLType::TIMESTAMP));

	// boolean columns
	REQUIRE_NO_FAIL(tester.Query("CREATE TABLE booleans(b BOOLEAN)"));
	REQUIRE_NO_FAIL(tester.Query("INSERT INTO booleans VALUES (42 > 60), (42 > 20), (42 > NULL)"));

	result = tester.Query("SELECT * FROM booleans ORDER BY b");
	REQUIRE_NO_FAIL(*result);
	REQUIRE(result->IsNull(0, 0));
	REQUIRE(!result->Fetch<bool>(0, 0));
	REQUIRE(!result->Fetch<bool>(0, 1));
	REQUIRE(result->Fetch<bool>(0, 2));
	REQUIRE(result->Fetch<string>(0, 2) == Value::BOOLEAN(true).ToString());
}

TEST_CASE("Test errors in C API", "[capi]") {
	CAPITester tester;
	unique_ptr<CAPIResult> result;

	// cannot open database in random directory
	REQUIRE(!tester.OpenDatabase("/bla/this/directory/should/not/exist/hopefully/awerar333"));
	REQUIRE(tester.OpenDatabase(nullptr));

	// syntax error in query
	REQUIRE_FAIL(tester.Query("SELEC * FROM TABLE"));
	// bind error
	REQUIRE_FAIL(tester.Query("SELECT * FROM TABLE"));

	graindb_result res;
	graindb_prepared_statement stmt = nullptr;
	// fail prepare API calls
	REQUIRE(graindb_prepare(NULL, "SELECT 42", &stmt) == GrainDBError);
	REQUIRE(graindb_prepare(tester.connection, NULL, &stmt) == GrainDBError);
	REQUIRE(graindb_bind_boolean(NULL, 0, true) == GrainDBError);
	REQUIRE(graindb_execute_prepared(NULL, &res) == GrainDBError);
	graindb_destroy_prepare(NULL);
}

TEST_CASE("Test prepared statements in C API", "[capi]") {
	CAPITester tester;
	unique_ptr<CAPIResult> result;
	graindb_result res;
	graindb_prepared_statement stmt = nullptr;
	graindb_state status;

	// open the database in in-memory mode
	REQUIRE(tester.OpenDatabase(nullptr));

	status = graindb_prepare(tester.connection, "SELECT CAST($1 AS BIGINT)", &stmt);
	REQUIRE(status == GrainDBSuccess);
	REQUIRE(stmt != nullptr);

	status = graindb_bind_boolean(stmt, 1, 1);
	REQUIRE(status == GrainDBSuccess);
	status = graindb_bind_boolean(stmt, 2, 1);
	REQUIRE(status == GrainDBError);

	status = graindb_execute_prepared(stmt, &res);
	REQUIRE(status == GrainDBSuccess);
	REQUIRE(graindb_value_int64(&res, 0, 0) == 1);
	graindb_destroy_result(&res);

	graindb_bind_int8(stmt, 1, 8);
	status = graindb_execute_prepared(stmt, &res);
	REQUIRE(status == GrainDBSuccess);
	REQUIRE(graindb_value_int64(&res, 0, 0) == 8);
	graindb_destroy_result(&res);

	graindb_bind_int16(stmt, 1, 16);
	status = graindb_execute_prepared(stmt, &res);
	REQUIRE(status == GrainDBSuccess);
	REQUIRE(graindb_value_int64(&res, 0, 0) == 16);
	graindb_destroy_result(&res);

	graindb_bind_int32(stmt, 1, 32);
	status = graindb_execute_prepared(stmt, &res);
	REQUIRE(status == GrainDBSuccess);
	REQUIRE(graindb_value_int64(&res, 0, 0) == 32);
	graindb_destroy_result(&res);

	graindb_bind_int64(stmt, 1, 64);
	status = graindb_execute_prepared(stmt, &res);
	REQUIRE(status == GrainDBSuccess);
	REQUIRE(graindb_value_int64(&res, 0, 0) == 64);
	graindb_destroy_result(&res);

	graindb_bind_float(stmt, 1, 42.0);
	status = graindb_execute_prepared(stmt, &res);
	REQUIRE(status == GrainDBSuccess);
	REQUIRE(graindb_value_int64(&res, 0, 0) == 42);
	graindb_destroy_result(&res);

	graindb_bind_double(stmt, 1, 43.0);
	status = graindb_execute_prepared(stmt, &res);
	REQUIRE(status == GrainDBSuccess);
	REQUIRE(graindb_value_int64(&res, 0, 0) == 43);
	graindb_destroy_result(&res);

	graindb_bind_varchar(stmt, 1, "44");
	status = graindb_execute_prepared(stmt, &res);
	REQUIRE(status == GrainDBSuccess);
	REQUIRE(graindb_value_int64(&res, 0, 0) == 44);
	graindb_destroy_result(&res);

	graindb_bind_null(stmt, 1);
	status = graindb_execute_prepared(stmt, &res);
	REQUIRE(status == GrainDBSuccess);
	REQUIRE(res.columns[0].nullmask[0] == true);
	graindb_destroy_result(&res);

	graindb_destroy_prepare(&stmt);
	// again to make sure it does not crash
	graindb_destroy_result(&res);
	graindb_destroy_prepare(&stmt);

	status = graindb_query(tester.connection, "CREATE TABLE a (i INTEGER)", NULL);
	REQUIRE(status == GrainDBSuccess);

	status = graindb_prepare(tester.connection, "INSERT INTO a VALUES (?)", &stmt);
	REQUIRE(status == GrainDBSuccess);
	REQUIRE(stmt != nullptr);
	idx_t nparams;
	REQUIRE(graindb_nparams(stmt, &nparams) == GrainDBSuccess);
	REQUIRE(nparams == 1);

	for (int32_t i = 1; i <= 1000; i++) {
		graindb_bind_int32(stmt, 1, i);
		status = graindb_execute_prepared(stmt, nullptr);
		REQUIRE(status == GrainDBSuccess);
	}
	graindb_destroy_prepare(&stmt);

	status = graindb_prepare(tester.connection, "SELECT SUM(i)*$1-$2 FROM a", &stmt);
	REQUIRE(status == GrainDBSuccess);
	REQUIRE(stmt != nullptr);
	graindb_bind_int32(stmt, 1, 2);
	graindb_bind_int32(stmt, 2, 1000);

	status = graindb_execute_prepared(stmt, &res);
	REQUIRE(status == GrainDBSuccess);
	REQUIRE(graindb_value_int32(&res, 0, 0) == 1000000);
	graindb_destroy_result(&res);
	graindb_destroy_prepare(&stmt);

	// not-so-happy path
	status = graindb_prepare(tester.connection, "SELECT XXXXX", &stmt);
	REQUIRE(status == GrainDBError);
	graindb_destroy_prepare(&stmt);

	status = graindb_prepare(tester.connection, "SELECT CAST($1 AS INTEGER)", &stmt);
	REQUIRE(status == GrainDBSuccess);
	REQUIRE(stmt != nullptr);

	status = graindb_execute_prepared(stmt, &res);
	REQUIRE(status == GrainDBError);
	graindb_destroy_result(&res);
	graindb_destroy_prepare(&stmt);
}
