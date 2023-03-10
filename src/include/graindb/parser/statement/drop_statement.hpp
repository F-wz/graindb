//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/statement/drop_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_data/drop_info.hpp"
#include "graindb/parser/sql_statement.hpp"

namespace graindb {

class DropStatement : public SQLStatement {
public:
	DropStatement() : SQLStatement(StatementType::DROP_STATEMENT), info(make_unique<DropInfo>()){};

	unique_ptr<DropInfo> info;
};

} // namespace graindb
