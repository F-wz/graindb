//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/optimizer/cse_optimizer.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/optimizer/rule.hpp"
#include "graindb/parser/expression_map.hpp"
#include "graindb/planner/logical_operator_visitor.hpp"

namespace graindb {
//! The CommonSubExpression optimizer traverses the expressions of a LogicalOperator to look for duplicate expressions,
//! and moves duplicate expressions into a shared CommonSubExpression.
class CommonSubExpressionOptimizer : public LogicalOperatorVisitor {
private:
	struct CSENode {
		idx_t count;
		Expression *expr;

		CSENode(idx_t count = 1, Expression *expr = nullptr) : count(count), expr(expr) {
		}
	};

	//! First iteration: count how many times each expression occurs
	void CountExpressions(Expression &expr, expression_map_t<CSENode> &expression_count);
	//! Second iteration: perform the actual replacement of the duplicate expressions with common subexpressions nodes
	void PerformCSEReplacement(unique_ptr<Expression> *expr, expression_map_t<CSENode> &expression_count);

	//! Main method to extract common subexpressions
	void ExtractCommonSubExpresions(LogicalOperator &op);

public:
	void VisitOperator(LogicalOperator &op) override;
};
} // namespace graindb
