//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/statement/vacuum_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_expression.hpp"
#include "graindb/parser/sql_statement.hpp"
#include "graindb/parser/parsed_data/vacuum_info.hpp"

namespace graindb {

class VacuumStatement : public SQLStatement {
public:
	VacuumStatement() : SQLStatement(StatementType::VACUUM_STATEMENT){};
	unique_ptr<VacuumInfo> info;
};

} // namespace graindb
