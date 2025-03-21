#include "graindb/execution/operator/join/physical_cross_product.hpp"
#include "graindb/execution/physical_plan_generator.hpp"
#include "graindb/planner/operator/logical_cross_product.hpp"

using namespace graindb;
using namespace std;

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalCrossProduct &op) {
	assert(op.children.size() == 2);

	auto left = CreatePlan(*op.children[0]);
	auto right = CreatePlan(*op.children[1]);
	return make_unique<PhysicalCrossProduct>(op, move(left), move(right));
}
