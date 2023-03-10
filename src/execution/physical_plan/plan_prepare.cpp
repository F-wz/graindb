#include "graindb/execution/operator/scan/physical_dummy_scan.hpp"
#include "graindb/execution/physical_plan_generator.hpp"
#include "graindb/planner/operator/logical_prepare.hpp"
#include "graindb/execution/operator/helper/physical_prepare.hpp"

using namespace graindb;
using namespace std;

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalPrepare &op) {
	assert(op.children.size() == 1);

	// generate physical plan
	auto plan = CreatePlan(*op.children[0]);
	op.prepared->types = plan->types;
	op.prepared->plan = move(plan);
	op.prepared->dependencies = move(dependencies);

	return make_unique<PhysicalPrepare>(op.name, move(op.prepared));
}
