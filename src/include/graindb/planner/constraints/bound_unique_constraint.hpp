//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/constraints/bound_unique_constraint.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/unordered_set.hpp"
#include "graindb/planner/bound_constraint.hpp"

namespace graindb {

class BoundUniqueConstraint : public BoundConstraint {
public:
	BoundUniqueConstraint(unordered_set<idx_t> keys, bool is_primary_key)
	    : BoundConstraint(ConstraintType::UNIQUE), keys(keys), is_primary_key(is_primary_key) {
	}

	//! The same keys but represented as an unordered set
	unordered_set<idx_t> keys;
	//! Whether or not the unique constraint is a primary key
	bool is_primary_key;
};

} // namespace graindb
