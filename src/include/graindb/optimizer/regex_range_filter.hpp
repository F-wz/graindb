//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/optimizer/regex_range_filter.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/optimizer/rule.hpp"

namespace graindb {

class Optimizer;

class RegexRangeFilter {
public:
	RegexRangeFilter() {
	}
	//! Perform filter pushdown
	unique_ptr<LogicalOperator> Rewrite(unique_ptr<LogicalOperator> node);
};

} // namespace graindb
