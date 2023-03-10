//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/bound_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/planner/logical_operator.hpp"

namespace graindb {

struct BoundStatement {
	unique_ptr<LogicalOperator> plan;
	vector<SQLType> types;
	vector<string> names;
};

} // namespace graindb
