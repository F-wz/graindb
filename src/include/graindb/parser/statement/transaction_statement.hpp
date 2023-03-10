//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/statement/transaction_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/sql_statement.hpp"
#include "graindb/parser/parsed_data/transaction_info.hpp"

namespace graindb {

class TransactionStatement : public SQLStatement {
public:
	TransactionStatement(TransactionType type)
	    : SQLStatement(StatementType::TRANSACTION_STATEMENT), info(make_unique<TransactionInfo>(type)){};

	unique_ptr<TransactionInfo> info;
};
} // namespace graindb
