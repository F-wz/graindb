//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/operator/logical_create.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/planner/logical_operator.hpp"
#include "graindb/parser/parsed_data/create_info.hpp"

namespace graindb {

//! LogicalCreate represents a CREATE operator
class LogicalCreate : public LogicalOperator {
public:
	LogicalCreate(LogicalOperatorType type, unique_ptr<CreateInfo> info, SchemaCatalogEntry *schema = nullptr)
	    : LogicalOperator(type), schema(schema), info(move(info)) {
	}

	SchemaCatalogEntry *schema;
	unique_ptr<CreateInfo> info;

protected:
	void ResolveTypes() override {
		types.push_back(TypeId::BOOL);
	}
};
} // namespace graindb
