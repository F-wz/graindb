#include "graindb/execution/operator/helper/physical_limit.hpp"
#include "graindb/execution/physical_plan_generator.hpp"
#include "graindb/planner/operator/logical_limit.hpp"

using namespace graindb;
using namespace std;

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalLimit &op) {
	assert(op.children.size() == 1);

	auto plan = CreatePlan(*op.children[0]);

	auto limit = make_unique<PhysicalLimit>(op, op.limit, op.offset);
	limit->children.push_back(move(plan));
	return move(limit);
}
