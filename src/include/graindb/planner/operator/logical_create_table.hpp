//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/operator/logical_create_table.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/planner/parsed_data/bound_create_table_info.hpp"
#include "graindb/planner/logical_operator.hpp"

namespace graindb {

class LogicalCreateTable : public LogicalOperator {
public:
	LogicalCreateTable(SchemaCatalogEntry *schema, unique_ptr<BoundCreateTableInfo> info)
	    : LogicalOperator(LogicalOperatorType::CREATE_TABLE), schema(schema), info(move(info)) {
	}

	//! Schema to insert to
	SchemaCatalogEntry *schema;
	//! Create Table information
	unique_ptr<BoundCreateTableInfo> info;

protected:
	void ResolveTypes() override {
		types.push_back(TypeId::INT64);
	}
};
} // namespace graindb
