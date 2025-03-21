//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/constraints/bound_not_null_constraint.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/planner/bound_constraint.hpp"

namespace graindb {

class BoundNotNullConstraint : public BoundConstraint {
public:
	BoundNotNullConstraint(column_t index) : BoundConstraint(ConstraintType::NOT_NULL), index(index) {
	}

	//! Column index this constraint pertains to
	column_t index;
};

} // namespace graindb
