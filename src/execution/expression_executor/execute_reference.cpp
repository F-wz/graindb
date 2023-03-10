#include "graindb/execution/expression_executor.hpp"
#include "graindb/planner/expression/bound_reference_expression.hpp"

using namespace graindb;
using namespace std;

unique_ptr<ExpressionState> ExpressionExecutor::InitializeState(BoundReferenceExpression &expr,
                                                                ExpressionExecutorState &root) {
	return nullptr;
}

void ExpressionExecutor::Execute(BoundReferenceExpression &expr, ExpressionState *state, const SelectionVector *sel,
                                 idx_t count, Vector &result) {
	assert(expr.index != INVALID_INDEX);
	assert(expr.index < chunk->column_count());
	if (sel) {
		result.Slice(chunk->data[expr.index], *sel, count);
	} else {
		result.Reference(chunk->data[expr.index]);
	}
}
