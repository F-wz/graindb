//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/optimizer/topn_optimizer.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/constants.hpp"

namespace graindb {
class LogicalOperator;
class Optimizer;

class TopN {
public:
	//! Optimize ORDER BY + LIMIT to TopN
	unique_ptr<LogicalOperator> Optimize(unique_ptr<LogicalOperator> node);
};

} // namespace graindb
