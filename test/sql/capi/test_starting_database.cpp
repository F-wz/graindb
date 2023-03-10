#include "catch.hpp"
#include "graindb.h"

using namespace std;

TEST_CASE("Simple In-Memory DB Start Up and Shutdown", "[simplestartup]") {
	graindb_database database;
	graindb_connection connection;

	// open and close a database in in-memory mode
	REQUIRE(graindb_open(NULL, &database) == GrainDBSuccess);
	REQUIRE(graindb_connect(database, &connection) == GrainDBSuccess);
	graindb_disconnect(&connection);
	graindb_close(&database);
}

TEST_CASE("Multiple In-Memory DB Start Up and Shutdown", "[multiplestartup]") {
	graindb_database database[10];
	graindb_connection connection[100];

	// open and close 10 databases
	// and open 10 connections per database
	for (size_t i = 0; i < 10; i++) {
		REQUIRE(graindb_open(NULL, &database[i]) == GrainDBSuccess);
		for (size_t j = 0; j < 10; j++) {
			REQUIRE(graindb_connect(database[i], &connection[i * 10 + j]) == GrainDBSuccess);
		}
	}
	for (size_t i = 0; i < 10; i++) {
		for (size_t j = 0; j < 10; j++) {
			graindb_disconnect(&connection[i * 10 + j]);
		}
		graindb_close(&database[i]);
	}
}
