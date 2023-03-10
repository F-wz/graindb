//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/statement/prepare_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_expression.hpp"
#include "graindb/parser/sql_statement.hpp"

namespace graindb {

class PrepareStatement : public SQLStatement {
public:
	PrepareStatement() : SQLStatement(StatementType::PREPARE_STATEMENT), statement(nullptr), name("") {
	}

	unique_ptr<SQLStatement> statement;
	string name;
};
} // namespace graindb
