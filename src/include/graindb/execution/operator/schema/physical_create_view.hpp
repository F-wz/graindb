//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/execution/operator/schema/physical_create_view.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/execution/physical_operator.hpp"
#include "graindb/parser/parsed_data/create_view_info.hpp"

namespace graindb {

//! PhysicalCreateView represents a CREATE VIEW command
class PhysicalCreateView : public PhysicalOperator {
public:
	PhysicalCreateView(unique_ptr<CreateViewInfo> info)
	    : PhysicalOperator(PhysicalOperatorType::CREATE_VIEW, {TypeId::BOOL}), info(move(info)) {
	}

	unique_ptr<CreateViewInfo> info;

public:
	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;
};

} // namespace graindb
