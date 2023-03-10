//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/parsed_data/bound_create_index_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/planner/parsed_data/bound_create_info.hpp"
#include "graindb/planner/bound_tableref.hpp"
#include "graindb/planner/expression.hpp"

namespace graindb {

struct BoundCreateIndexInfo : public BoundCreateInfo {
	BoundCreateIndexInfo(unique_ptr<CreateInfo> base) : BoundCreateInfo(move(base)) {
	}

	//! The table to index
	unique_ptr<BoundTableRef> table;
	//! Set of expressions to index by
	vector<unique_ptr<Expression>> expressions;
};

} // namespace graindb
