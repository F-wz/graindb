//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/execution/operator/helper/physical_prepare.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/enums/physical_operator_type.hpp"
#include "graindb/execution/physical_operator.hpp"
#include "graindb/main/prepared_statement_data.hpp"

namespace graindb {

class PhysicalPrepare : public PhysicalOperator {
public:
	PhysicalPrepare(string name, unique_ptr<PreparedStatementData> prepared)
	    : PhysicalOperator(PhysicalOperatorType::PREPARE, {TypeId::BOOL}), name(name), prepared(move(prepared)) {
	}

	string name;
	unique_ptr<PreparedStatementData> prepared;

public:
	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;
};

} // namespace graindb
