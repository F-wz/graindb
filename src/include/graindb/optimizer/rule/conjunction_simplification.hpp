//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/optimizer/rule/conjunction_simplification.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/optimizer/rule.hpp"

namespace graindb {

// The Conjunction Simplification rule rewrites conjunctions with a constant
class ConjunctionSimplificationRule : public Rule {
public:
	ConjunctionSimplificationRule(ExpressionRewriter &rewriter);

	unique_ptr<Expression> Apply(LogicalOperator &op, vector<Expression *> &bindings, bool &changes_made) override;

	unique_ptr<Expression> RemoveExpression(BoundConjunctionExpression &conj, Expression *expr);
};

} // namespace graindb
