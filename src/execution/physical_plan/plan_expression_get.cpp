#include "graindb/execution/operator/scan/physical_expression_scan.hpp"
#include "graindb/execution/physical_plan_generator.hpp"
#include "graindb/planner/operator/logical_expression_get.hpp"

using namespace graindb;
using namespace std;

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalExpressionGet &op) {
	assert(op.children.size() == 1);
	auto plan = CreatePlan(*op.children[0]);

	auto expr_scan = make_unique<PhysicalExpressionScan>(op.types, move(op.expressions));
	expr_scan->children.push_back(move(plan));
	return move(expr_scan);
}
