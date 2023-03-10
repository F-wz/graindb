//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/function/cast_rules.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/types.hpp"

namespace graindb {
//! Contains a list of rules for casting
class CastRules {
public:
	//! Returns the cost of performing an implicit cost from "from" to "to", or -1 if an implicit cast is not possible
	static int64_t ImplicitCast(SQLType from, SQLType to);
};

} // namespace graindb
