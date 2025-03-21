//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/execution/operator/aggregate/physical_window.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/types/chunk_collection.hpp"
#include "graindb/execution/physical_operator.hpp"

namespace graindb {

//! PhysicalWindow implements window functions
class PhysicalWindow : public PhysicalOperator {
public:
	PhysicalWindow(LogicalOperator &op, vector<unique_ptr<Expression>> select_list,
	               PhysicalOperatorType type = PhysicalOperatorType::WINDOW);

	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;

	//! The projection list of the SELECT statement (that contains aggregates)
	vector<unique_ptr<Expression>> select_list;

public:
	unique_ptr<PhysicalOperatorState> GetOperatorState() override;
};

} // namespace graindb
