/// @file
/// @brief schema and dut classes for GrainDB

#ifndef GRAINDB_HH
#define GRAINDB_HH

#include "graindb.hpp"

#include "dut.hh"
#include "relmodel.hh"
#include "schema.hh"

struct graindb_connection {
	std::unique_ptr<graindb::GrainDB> database;
	std::unique_ptr<graindb::Connection> connection;
	char *zErrMsg = 0;
	int rc;
	void q(const char *query);
	graindb_connection(std::string &conninfo);
};

struct schema_graindb : schema, graindb_connection {
	schema_graindb(std::string &conninfo, bool no_catalog);
	virtual std::string quote_name(const std::string &id) {
		return id;
	}
};

struct dut_graindb : dut_base, graindb_connection {
	virtual void test(const std::string &stmt);
	dut_graindb(std::string &conninfo);
};
#endif
