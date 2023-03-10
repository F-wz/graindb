#include "graindb/execution/operator/scan/physical_empty_result.hpp"
#include "graindb/execution/physical_plan_generator.hpp"
#include "graindb/planner/operator/logical_empty_result.hpp"

using namespace graindb;
using namespace std;

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalEmptyResult &op) {
	assert(op.children.size() == 0);
	return make_unique<PhysicalEmptyResult>(op.types);
}
