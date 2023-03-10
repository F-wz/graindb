//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/parsed_data/bound_create_rai_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/enums/rai_direction.hpp"
#include "graindb/planner/bound_tableref.hpp"
#include "graindb/planner/expression.hpp"
#include "graindb/planner/parsed_data/bound_create_info.hpp"

namespace graindb {

struct BoundCreateRAIInfo : public BoundCreateInfo {
	BoundCreateRAIInfo(unique_ptr<CreateInfo> base) : BoundCreateInfo(move(base)) {
	}

	string name;
	unique_ptr<BoundTableRef> table;
	RAIDirection rai_direction;
	vector<unique_ptr<BoundTableRef>> referenced_tables;
	vector<unique_ptr<Expression>> columns;
	vector<unique_ptr<Expression>> references;
	vector<column_t> base_column_ids;
	vector<column_t> referenced_column_ids;
	unique_ptr<LogicalOperator> plan;
};
} // namespace graindb
