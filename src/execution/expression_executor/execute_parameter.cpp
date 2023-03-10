#include "graindb/common/vector_operations/vector_operations.hpp"
#include "graindb/execution/expression_executor.hpp"
#include "graindb/planner/expression/bound_parameter_expression.hpp"

using namespace graindb;
using namespace std;

unique_ptr<ExpressionState> ExpressionExecutor::InitializeState(BoundParameterExpression &expr,
                                                                ExpressionExecutorState &root) {
	return nullptr;
}

void ExpressionExecutor::Execute(BoundParameterExpression &expr, ExpressionState *state, const SelectionVector *sel,
                                 idx_t count, Vector &result) {
	assert(expr.value);
	assert(expr.value->type == expr.return_type);
	result.Reference(*expr.value);
}
