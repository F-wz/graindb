//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/execution/operator/schema/physical_create_schema.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/execution/physical_operator.hpp"
#include "graindb/parser/parsed_data/create_schema_info.hpp"

namespace graindb {

//! PhysicalCreateSchema represents a CREATE SCHEMA command
class PhysicalCreateSchema : public PhysicalOperator {
public:
	PhysicalCreateSchema(unique_ptr<CreateSchemaInfo> info)
	    : PhysicalOperator(PhysicalOperatorType::CREATE_SCHEMA, {TypeId::BOOL}), info(move(info)) {
	}

	unique_ptr<CreateSchemaInfo> info;

public:
	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;
};

} // namespace graindb
