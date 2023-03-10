#include "graindb/common/vector_operations/vector_operations.hpp"
#include "graindb/execution/expression_executor.hpp"
#include "graindb/planner/expression/bound_constant_expression.hpp"

using namespace graindb;
using namespace std;

unique_ptr<ExpressionState> ExpressionExecutor::InitializeState(BoundConstantExpression &expr,
                                                                ExpressionExecutorState &root) {
	return nullptr;
}

void ExpressionExecutor::Execute(BoundConstantExpression &expr, ExpressionState *state, const SelectionVector *sel,
                                 idx_t count, Vector &result) {
	result.Reference(expr.value);
}
