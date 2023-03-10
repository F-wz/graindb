//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/execution/operator/helper/physical_limit.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/execution/physical_operator.hpp"

namespace graindb {

//! PhyisicalLimit represents the LIMIT operator
class PhysicalLimit : public PhysicalOperator {
public:
	PhysicalLimit(LogicalOperator &op, idx_t limit, idx_t offset)
	    : PhysicalOperator(PhysicalOperatorType::LIMIT, op.types), limit(limit), offset(offset) {
	}

	idx_t limit;
	idx_t offset;

public:
	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;

	unique_ptr<PhysicalOperatorState> GetOperatorState() override;
};

} // namespace graindb
