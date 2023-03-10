//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/statement/bound_create_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/planner/parsed_data/bound_create_info.hpp"
#include "graindb/planner/bound_sql_statement.hpp"

namespace graindb {
//! Bound equivalent to CreateStatement
class BoundCreateStatement : public BoundSQLStatement {
public:
	BoundCreateStatement() : BoundSQLStatement(StatementType::CREATE) {
	}

	// Info for element creation
	unique_ptr<BoundCreateInfo> info;

public:
	vector<string> GetNames() override {
		return {"Count"};
	}
	vector<SQLType> GetTypes() override {
		return {SQLType::BIGINT};
	}
};
} // namespace graindb
