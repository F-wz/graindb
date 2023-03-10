//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/sql_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/common.hpp"
#include "graindb/common/enums/statement_type.hpp"
#include "graindb/common/exception.hpp"
#include "graindb/common/printer.hpp"

namespace graindb {
//! SQLStatement is the base class of any type of SQL statement.
class SQLStatement {
public:
	SQLStatement(StatementType type) : type(type){};
	virtual ~SQLStatement() {
	}

	StatementType type;
	idx_t stmt_location;
	idx_t stmt_length;
};
} // namespace graindb
