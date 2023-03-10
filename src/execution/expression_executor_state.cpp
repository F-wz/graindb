#include "graindb/execution/expression_executor_state.hpp"
#include "graindb/execution/expression_executor.hpp"
#include "graindb/planner/expression.hpp"

using namespace graindb;
using namespace std;

void ExpressionState::AddChild(Expression *expr) {
	child_states.push_back(ExpressionExecutor::InitializeState(*expr, root));
}
