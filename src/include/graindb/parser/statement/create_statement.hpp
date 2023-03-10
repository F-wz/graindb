//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/statement/create_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_data/create_info.hpp"
#include "graindb/parser/sql_statement.hpp"

namespace graindb {

class CreateStatement : public SQLStatement {
public:
	CreateStatement() : SQLStatement(StatementType::CREATE_STATEMENT){};

	unique_ptr<CreateInfo> info;
};

} // namespace graindb
