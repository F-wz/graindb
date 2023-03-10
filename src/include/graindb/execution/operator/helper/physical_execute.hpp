//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/execution/operator/helper/physical_execute.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/execution/physical_operator.hpp"

namespace graindb {

class PhysicalExecute : public PhysicalOperator {
public:
	PhysicalExecute(PhysicalOperator *plan) : PhysicalOperator(PhysicalOperatorType::EXECUTE, plan->types), plan(plan) {
	}

	PhysicalOperator *plan;

public:
	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;

	unique_ptr<PhysicalOperatorState> GetOperatorState() override;
};

} // namespace graindb
