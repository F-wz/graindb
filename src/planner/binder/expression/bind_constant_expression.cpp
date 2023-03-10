#include "graindb/parser/expression/constant_expression.hpp"
#include "graindb/planner/expression/bound_constant_expression.hpp"
#include "graindb/planner/expression_binder.hpp"

using namespace graindb;
using namespace std;

BindResult ExpressionBinder::BindExpression(ConstantExpression &expr, idx_t depth) {
	return BindResult(make_unique<BoundConstantExpression>(expr.value), expr.sql_type);
}
