//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/statement/execute_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_expression.hpp"
#include "graindb/parser/sql_statement.hpp"

namespace graindb {

class ExecuteStatement : public SQLStatement {
public:
	ExecuteStatement() : SQLStatement(StatementType::EXECUTE_STATEMENT){};

	string name;
	vector<unique_ptr<ParsedExpression>> values;
};
} // namespace graindb
