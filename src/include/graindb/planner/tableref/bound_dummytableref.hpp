//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/tableref/bound_dummytableref.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/planner/bound_tableref.hpp"

namespace graindb {

//! Represents a cross product
class BoundEmptyTableRef : public BoundTableRef {
public:
	BoundEmptyTableRef(idx_t bind_index) : BoundTableRef(TableReferenceType::EMPTY), bind_index(bind_index) {
	}
	idx_t bind_index;
};
} // namespace graindb
