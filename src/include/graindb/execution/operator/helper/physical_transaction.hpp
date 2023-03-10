//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/execution/operator/helper/physical_transaction.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/execution/physical_operator.hpp"
#include "graindb/parser/parsed_data/transaction_info.hpp"

namespace graindb {

//! PhysicalTransaction represents a transaction operator (e.g. BEGIN or COMMIT)
class PhysicalTransaction : public PhysicalOperator {
public:
	PhysicalTransaction(unique_ptr<TransactionInfo> info)
	    : PhysicalOperator(PhysicalOperatorType::TRANSACTION, {TypeId::BOOL}), info(move(info)) {
	}

	unique_ptr<TransactionInfo> info;

public:
	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;
};

} // namespace graindb
