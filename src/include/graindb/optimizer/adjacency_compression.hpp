//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/optimizer/adjacency_compression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/planner/logical_operator.hpp"
#include "graindb/planner/operator/logical_comparison_join.hpp"
#include "graindb/planner/operator/logical_get.hpp"

namespace graindb {

class Optimizer;

class AdjacencyCompression {
public:
	unique_ptr<LogicalOperator> Optimize(unique_ptr<LogicalOperator> op);

private:
	unique_ptr<LogicalOperator> Compress(unique_ptr<LogicalOperator> op);
};

} // namespace graindb
