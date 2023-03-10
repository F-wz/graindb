//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/execution/operator/order/physical_top_n.hpp
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
class PhysicalTopN : public PhysicalOperator {
public:
	PhysicalTopN(LogicalOperator &op, vector<BoundOrderByNode> orders, idx_t limit, idx_t offset)
	    : PhysicalOperator(PhysicalOperatorType::TOP_N, op.types), orders(move(orders)), limit(limit), offset(offset),
	      heap_size(0) {
	}

	vector<BoundOrderByNode> orders;
	idx_t limit;
	idx_t offset;
	idx_t heap_size;

public:
	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;
	unique_ptr<PhysicalOperatorState> GetOperatorState() override;
	void CalculateHeapSize(idx_t rows);
};

} // namespace graindb
