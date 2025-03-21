#include "graindb/optimizer/regex_range_filter.hpp"

#include "graindb/catalog/catalog_entry/scalar_function_catalog_entry.hpp"

#include "graindb/function/scalar/string_functions.hpp"

#include "graindb/planner/expression.hpp"
#include "graindb/planner/expression/bound_comparison_expression.hpp"
#include "graindb/planner/expression/bound_conjunction_expression.hpp"
#include "graindb/planner/expression/bound_constant_expression.hpp"
#include "graindb/planner/expression/bound_function_expression.hpp"
#include "graindb/planner/operator/logical_filter.hpp"

using namespace graindb;
using namespace std;

unique_ptr<LogicalOperator> RegexRangeFilter::Rewrite(unique_ptr<LogicalOperator> op) {

	for (idx_t child_idx = 0; child_idx < op->children.size(); child_idx++) {
		op->children[child_idx] = Rewrite(move(op->children[child_idx]));
	}

	if (op->type != LogicalOperatorType::FILTER) {
		return op;
	}

	auto new_filter = make_unique<LogicalFilter>();

	for (auto &expr : op->expressions) {
		if (expr->type == ExpressionType::BOUND_FUNCTION) {
			auto &func = (BoundFunctionExpression &)*expr.get();
			if (func.function.name != "regexp_full_match" || func.children.size() != 2) {
				continue;
			}
			auto &info = (RegexpMatchesBindData &)*func.bind_info;
			if (!info.range_success) {
				continue;
			}
			auto filter_left = make_unique<BoundComparisonExpression>(
			    ExpressionType::COMPARE_GREATERTHANOREQUALTO, func.children[0]->Copy(),
			    make_unique<BoundConstantExpression>(Value::BLOB(info.range_min)));
			auto filter_right = make_unique<BoundComparisonExpression>(
			    ExpressionType::COMPARE_LESSTHANOREQUALTO, func.children[0]->Copy(),
			    make_unique<BoundConstantExpression>(Value::BLOB(info.range_max)));
			auto filter_expr = make_unique<BoundConjunctionExpression>(ExpressionType::CONJUNCTION_AND,
			                                                           move(filter_left), move(filter_right));

			new_filter->expressions.push_back(move(filter_expr));
		}
	}

	if (new_filter->expressions.size() > 0) {
		new_filter->children = move(op->children);
		op->children.clear();
		op->children.push_back(move(new_filter));
	}

	return op;
}
