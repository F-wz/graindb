#include "graindb/parser/expression/parameter_expression.hpp"
#include "graindb/planner/binder.hpp"
#include "graindb/planner/expression/bound_parameter_expression.hpp"
#include "graindb/planner/expression_binder.hpp"

using namespace graindb;
using namespace std;

BindResult ExpressionBinder::BindExpression(ParameterExpression &expr, idx_t depth) {
	auto bound_parameter = make_unique<BoundParameterExpression>(expr.parameter_nr);
	auto sql_type = bound_parameter->sql_type;
	binder.parameters->push_back(bound_parameter.get());
	return BindResult(move(bound_parameter), sql_type);
}
