//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/execution/operator/scan/physical_empty_result.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/execution/physical_operator.hpp"

namespace graindb {

class PhysicalEmptyResult : public PhysicalOperator {
public:
	PhysicalEmptyResult(vector<TypeId> types) : PhysicalOperator(PhysicalOperatorType::EMPTY_RESULT, types) {
	}

public:
	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;
};
} // namespace graindb
