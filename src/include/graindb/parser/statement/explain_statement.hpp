//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/statement/explain_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_expression.hpp"
#include "graindb/parser/sql_statement.hpp"

namespace graindb {

class ExplainStatement : public SQLStatement {
public:
	ExplainStatement(unique_ptr<SQLStatement> stmt) : SQLStatement(StatementType::EXPLAIN_STATEMENT), stmt(move(stmt)){};

	unique_ptr<SQLStatement> stmt;
};

} // namespace graindb
