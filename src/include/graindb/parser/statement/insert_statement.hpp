//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/statement/insert_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_expression.hpp"
#include "graindb/parser/statement/select_statement.hpp"

#include <vector>

namespace graindb {

class InsertStatement : public SQLStatement {
public:
	InsertStatement() : SQLStatement(StatementType::INSERT_STATEMENT), schema(DEFAULT_SCHEMA){};

	//! The select statement to insert from
	unique_ptr<SelectStatement> select_statement;
	//! Column names to insert into
	vector<string> columns;

	//! Table name to insert to
	string table;
	//! Schema name to insert to
	string schema;
};

} // namespace graindb
