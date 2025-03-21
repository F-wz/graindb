//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/execution/operator/join/physical_cross_product.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/types/chunk_collection.hpp"
#include "graindb/execution/physical_operator.hpp"

namespace graindb {
//! PhysicalCrossProduct represents a cross product between two tables
class PhysicalCrossProduct : public PhysicalOperator {
public:
	PhysicalCrossProduct(LogicalOperator &op, unique_ptr<PhysicalOperator> left, unique_ptr<PhysicalOperator> right);

public:
	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;

	unique_ptr<PhysicalOperatorState> GetOperatorState() override;
};

} // namespace graindb
