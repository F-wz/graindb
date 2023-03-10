//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/execution/operator/persistent/physical_copy_to_file.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/execution/physical_operator.hpp"
#include "graindb/parser/parsed_data/copy_info.hpp"

namespace graindb {

//! Copy the contents of a query into a table
class PhysicalCopyToFile : public PhysicalOperator {
public:
	PhysicalCopyToFile(LogicalOperator &op, unique_ptr<CopyInfo> info)
	    : PhysicalOperator(PhysicalOperatorType::COPY_TO_FILE, op.types), info(move(info)) {
	}

	//! Settings for the COPY statement
	unique_ptr<CopyInfo> info;
	//! The names of the child expression
	vector<string> names;
	//! The types of the child expression
	vector<SQLType> sql_types;

public:
	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;
};
} // namespace graindb
