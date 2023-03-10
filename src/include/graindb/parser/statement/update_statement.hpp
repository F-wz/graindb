//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/statement/update_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_expression.hpp"
#include "graindb/parser/sql_statement.hpp"
#include "graindb/parser/tableref.hpp"

namespace graindb {

class UpdateStatement : public SQLStatement {
public:
	UpdateStatement() : SQLStatement(StatementType::UPDATE_STATEMENT) {
	}

	unique_ptr<ParsedExpression> condition;
	unique_ptr<TableRef> table;

	vector<string> columns;
	vector<unique_ptr<ParsedExpression>> expressions;
};
} // namespace graindb
