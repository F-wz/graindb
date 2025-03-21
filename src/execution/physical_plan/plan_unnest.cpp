#include "graindb/execution/operator/projection/physical_unnest.hpp"
#include "graindb/execution/physical_plan_generator.hpp"
#include "graindb/planner/operator/logical_unnest.hpp"

using namespace graindb;
using namespace std;

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalUnnest &op) {
	assert(op.children.size() == 1);
	auto plan = CreatePlan(*op.children[0]);
	auto unnest = make_unique<PhysicalUnnest>(op, move(op.expressions));
	unnest->children.push_back(move(plan));
	return move(unnest);
}
