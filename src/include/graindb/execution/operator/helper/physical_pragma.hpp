//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/execution/operator/helper/physical_pragma.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/execution/physical_operator.hpp"
#include "graindb/parser/parsed_data/pragma_info.hpp"

namespace graindb {

//! PhysicalPragma represents the PRAGMA operator
class PhysicalPragma : public PhysicalOperator {
public:
	PhysicalPragma(unique_ptr<PragmaInfo> info)
	    : PhysicalOperator(PhysicalOperatorType::PRAGMA, {TypeId::BOOL}), info(move(info)) {
	}

	unique_ptr<PragmaInfo> info;

public:
	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;
};

} // namespace graindb
