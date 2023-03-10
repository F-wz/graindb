//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/statement/alter_table_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/column_definition.hpp"
#include "graindb/parser/parsed_data/alter_table_info.hpp"
#include "graindb/parser/sql_statement.hpp"

namespace graindb {

class AlterTableStatement : public SQLStatement {
public:
	AlterTableStatement() : SQLStatement(StatementType::ALTER_STATEMENT) {
	}
	AlterTableStatement(unique_ptr<AlterTableInfo> info)
	    : SQLStatement(StatementType::ALTER_STATEMENT), info(std::move(info)) {
	}

	unique_ptr<AlterTableInfo> info;
};

} // namespace graindb
