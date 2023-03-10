//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/bound_constraint.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/common.hpp"
#include "graindb/parser/constraint.hpp"

namespace graindb {
//! Bound equivalent of Constraint
class BoundConstraint {
public:
	BoundConstraint(ConstraintType type) : type(type){};
	virtual ~BoundConstraint() {
	}

	ConstraintType type;
};
} // namespace graindb
