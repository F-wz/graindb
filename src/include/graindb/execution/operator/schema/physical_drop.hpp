//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/execution/operator/schema/physical_drop.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/execution/physical_operator.hpp"
#include "graindb/parser/parsed_data/drop_info.hpp"

namespace graindb {

//! PhysicalDrop represents a DROP [...] command
class PhysicalDrop : public PhysicalOperator {
public:
	PhysicalDrop(unique_ptr<DropInfo> info)
	    : PhysicalOperator(PhysicalOperatorType::DROP, {TypeId::BOOL}), info(move(info)) {
	}

	unique_ptr<DropInfo> info;

public:
	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;
};

} // namespace graindb
