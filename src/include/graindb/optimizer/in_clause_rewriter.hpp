//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/optimizer/in_clause_rewriter.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/planner/logical_operator_visitor.hpp"

namespace graindb {
class Optimizer;

class InClauseRewriter : public LogicalOperatorVisitor {
public:
	InClauseRewriter(Optimizer &optimizer) : optimizer(optimizer) {
	}

	Optimizer &optimizer;
	unique_ptr<LogicalOperator> root;

public:
	unique_ptr<LogicalOperator> Rewrite(unique_ptr<LogicalOperator> op);

	unique_ptr<Expression> VisitReplace(BoundOperatorExpression &expr, unique_ptr<Expression> *expr_ptr) override;
};

} // namespace graindb
