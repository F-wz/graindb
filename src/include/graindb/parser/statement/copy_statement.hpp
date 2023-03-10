//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/statement/copy_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_data/copy_info.hpp"
#include "graindb/parser/query_node.hpp"
#include "graindb/parser/sql_statement.hpp"

#include <vector>

namespace graindb {

class CopyStatement : public SQLStatement {
public:
	CopyStatement() : SQLStatement(StatementType::COPY_STATEMENT), info(make_unique<CopyInfo>()){};

	unique_ptr<CopyInfo> info;
	// The SQL statement used instead of a table when copying data out to a file
	unique_ptr<QueryNode> select_statement;
};
} // namespace graindb
