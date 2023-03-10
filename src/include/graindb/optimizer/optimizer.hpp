//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/optimizer/optimizer.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/optimizer/expression_rewriter.hpp"
#include "graindb/planner/logical_operator.hpp"
#include "graindb/planner/logical_operator_visitor.hpp"

namespace graindb {
class Binder;

class Optimizer {
public:
	Optimizer(Binder &binder, ClientContext &context);

	unique_ptr<LogicalOperator> Optimize(unique_ptr<LogicalOperator> plan);

	ClientContext &context;
	Binder &binder;
	ExpressionRewriter rewriter;
};

} // namespace graindb
