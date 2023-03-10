//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/statement/pragma_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/sql_statement.hpp"
#include "graindb/parser/parsed_data/pragma_info.hpp"

namespace graindb {

class PragmaStatement : public SQLStatement {
public:
	PragmaStatement() : SQLStatement(StatementType::PRAGMA_STATEMENT), info(make_unique<PragmaInfo>()){};

	unique_ptr<PragmaInfo> info;
};

} // namespace graindb
