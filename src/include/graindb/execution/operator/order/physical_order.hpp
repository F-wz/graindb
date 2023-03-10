//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/execution/operator/order/physical_order.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/types/chunk_collection.hpp"
#include "graindb/execution/physical_operator.hpp"
#include "graindb/planner/bound_query_node.hpp"

namespace graindb {

//! Represents a physical ordering of the data. Note that this will not change
//! the data but only add a selection vector.
class PhysicalOrder : public PhysicalOperator {
public:
	PhysicalOrder(vector<TypeId> types, vector<BoundOrderByNode> orders)
	    : PhysicalOperator(PhysicalOperatorType::ORDER_BY, move(types)), orders(move(orders)) {
	}

	vector<BoundOrderByNode> orders;

public:
	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;
	unique_ptr<PhysicalOperatorState> GetOperatorState() override;
};

} // namespace graindb
