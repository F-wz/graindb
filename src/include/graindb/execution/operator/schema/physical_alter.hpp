//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/execution/operator/schema/physical_alter.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/execution/physical_operator.hpp"
#include "graindb/parser/parsed_data/alter_table_info.hpp"

namespace graindb {

//! PhysicalAlter represents an ALTER TABLE command
class PhysicalAlter : public PhysicalOperator {
public:
	PhysicalAlter(unique_ptr<AlterInfo> info)
	    : PhysicalOperator(PhysicalOperatorType::ALTER, {TypeId::BOOL}), info(move(info)) {
	}

	unique_ptr<AlterInfo> info;

public:
	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;
};

} // namespace graindb
