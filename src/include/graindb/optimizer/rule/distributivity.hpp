//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/optimizer/rule/distributivity.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/optimizer/rule.hpp"
#include "graindb/parser/expression_map.hpp"

namespace graindb {

// (X AND B) OR (X AND C) OR (X AND D) = X AND (B OR C OR D)
class DistributivityRule : public Rule {
public:
	DistributivityRule(ExpressionRewriter &rewriter);

	unique_ptr<Expression> Apply(LogicalOperator &op, vector<Expression *> &bindings, bool &changes_made) override;

private:
	void AddExpressionSet(Expression &expr, expression_set_t &set);
	unique_ptr<Expression> ExtractExpression(BoundConjunctionExpression &conj, idx_t idx, Expression &expr);
};

} // namespace graindb
