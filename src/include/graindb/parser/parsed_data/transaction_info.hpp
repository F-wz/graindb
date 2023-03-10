//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/parsed_data/transaction_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_data/parse_info.hpp"

namespace graindb {

enum class TransactionType : uint8_t { INVALID, BEGIN_TRANSACTION, COMMIT, ROLLBACK };

struct TransactionInfo : public ParseInfo {
	TransactionInfo(TransactionType type) : type(type) {
	}

	//! The type of transaction statement
	TransactionType type;
};

} // namespace graindb
