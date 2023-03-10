#include "graindb/optimizer/rule/constant_folding.hpp"

#include "graindb/common/exception.hpp"
#include "graindb/execution/expression_executor.hpp"
#include "graindb/optimizer/expression_rewriter.hpp"
#include "graindb/planner/expression/bound_constant_expression.hpp"

using namespace graindb;
using namespace std;

namespace graindb {

//! The ConstantFoldingExpressionMatcher matches on any scalar expression (i.e. Expression::IsFoldable is true)
class ConstantFoldingExpressionMatcher : public FoldableConstantMatcher {
public:
	bool Match(Expression *expr, vector<Expression *> &bindings) override {
		// we also do not match on ConstantExpressions, because we cannot fold those any further
		if (expr->type == ExpressionType::VALUE_CONSTANT) {
			return false;
		}
		return FoldableConstantMatcher::Match(expr, bindings);
	}
};
} // namespace graindb

ConstantFoldingRule::ConstantFoldingRule(ExpressionRewriter &rewriter) : Rule(rewriter) {
	auto op = make_unique<ConstantFoldingExpressionMatcher>();
	root = move(op);
}

unique_ptr<Expression> ConstantFoldingRule::Apply(LogicalOperator &op, vector<Expression *> &bindings,
                                                  bool &changes_made) {
	auto root = bindings[0];
	// the root is a scalar expression that we have to fold
	assert(root->IsFoldable() && root->type != ExpressionType::VALUE_CONSTANT);

	// use an ExpressionExecutor to execute the expression
	auto result_value = ExpressionExecutor::EvaluateScalar(*root);
	assert(result_value.type == root->return_type);
	// now get the value from the result vector and insert it back into the plan as a constant expression
	return make_unique<BoundConstantExpression>(result_value);
}
