//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/bound_tableref.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/common.hpp"
#include "graindb/common/enums/tableref_type.hpp"

namespace graindb {

class BoundTableRef {
public:
	BoundTableRef(TableReferenceType type) : type(type) {
	}
	virtual ~BoundTableRef() {
	}

	//! The type of table reference
	TableReferenceType type;
};
} // namespace graindb
