//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/statement/delete_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_expression.hpp"
#include "graindb/parser/sql_statement.hpp"
#include "graindb/parser/tableref.hpp"

namespace graindb {

class DeleteStatement : public SQLStatement {
public:
	DeleteStatement() : SQLStatement(StatementType::DELETE_STATEMENT) {
	}

	unique_ptr<ParsedExpression> condition;
	unique_ptr<TableRef> table;
};
} // namespace graindb
