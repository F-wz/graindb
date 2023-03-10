#include "graindb/execution/operator/scan/physical_index_scan.hpp"
#include "graindb/execution/physical_plan_generator.hpp"
#include "graindb/planner/operator/logical_index_scan.hpp"

using namespace graindb;
using namespace std;

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalIndexScan &op) {
	unique_ptr<PhysicalOperator> plan;
	auto node = make_unique<PhysicalIndexScan>(op, op.tableref, op.table, op.index, op.column_ids, op.table_index);
	if (op.equal_index) {
		node->equal_value = op.equal_value;
		node->equal_index = true;
	}
	if (op.low_index) {
		node->low_value = op.low_value;
		node->low_index = true;
		node->low_expression_type = op.low_expression_type;
	}
	if (op.high_index) {
		node->high_value = op.high_value;
		node->high_index = true;
		node->high_expression_type = op.high_expression_type;
	}
	plan = move(node);
	return plan;
}
