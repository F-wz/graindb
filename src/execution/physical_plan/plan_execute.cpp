#include "graindb/execution/operator/helper/physical_execute.hpp"
#include "graindb/execution/physical_plan_generator.hpp"
#include "graindb/planner/operator/logical_execute.hpp"

using namespace graindb;
using namespace std;

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalExecute &op) {
	assert(op.children.size() == 0);
	return make_unique<PhysicalExecute>(op.prepared->plan.get());
}
