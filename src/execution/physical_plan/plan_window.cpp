#include "graindb/execution/operator/aggregate/physical_window.hpp"
#include "graindb/execution/physical_plan_generator.hpp"
#include "graindb/planner/operator/logical_window.hpp"

using namespace graindb;
using namespace std;

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalWindow &op) {
	assert(op.children.size() == 1);

	auto plan = CreatePlan(*op.children[0]);
#ifdef DEBUG
	for (auto &expr : op.expressions) {
		assert(expr->IsWindow());
	}
#endif

	auto window = make_unique<PhysicalWindow>(op, move(op.expressions));
	window->children.push_back(move(plan));
	return move(window);
}
