#include "graindb/execution/operator/projection/physical_projection.hpp"
#include "graindb/execution/physical_plan_generator.hpp"
#include "graindb/planner/expression/bound_reference_expression.hpp"
#include "graindb/planner/operator/logical_projection.hpp"

using namespace graindb;
using namespace std;

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalProjection &op) {
	assert(op.children.size() == 1);
	auto plan = CreatePlan(*op.children[0]);

#ifdef DEBUG
	for (auto &expr : op.expressions) {
		assert(!expr->IsWindow() && !expr->IsAggregate());
	}
#endif

	auto projection = make_unique<PhysicalProjection>(op, move(op.expressions));
	projection->children.push_back(move(plan));
	return move(projection);
}
