#include "graindb/execution/operator/order/physical_order.hpp"
#include "graindb/execution/physical_plan_generator.hpp"
#include "graindb/planner/operator/logical_order.hpp"

using namespace graindb;
using namespace std;

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalOrder &op) {
	assert(op.children.size() == 1);

	auto plan = CreatePlan(*op.children[0]);
	if (op.orders.size() > 0) {
		auto order = make_unique<PhysicalOrder>(op.types, move(op.orders));
		order->children.push_back(move(plan));
		plan = move(order);
	}
	return plan;
}
