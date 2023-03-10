#include "graindb/catalog/catalog_entry/table_catalog_entry.hpp"
#include "graindb/execution/operator/persistent/physical_insert.hpp"
#include "graindb/execution/physical_plan_generator.hpp"
#include "graindb/planner/operator/logical_insert.hpp"

using namespace graindb;
using namespace std;

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalInsert &op) {
	unique_ptr<PhysicalOperator> plan;
	if (op.children.size() > 0) {
		assert(op.children.size() == 1);
		plan = CreatePlan(*op.children[0]);
	}

	dependencies.insert(op.table);
	auto insert = make_unique<PhysicalInsert>(op, op.table, op.column_index_map, move(op.bound_defaults));
	if (plan) {
		insert->children.push_back(move(plan));
	}
	return move(insert);
}
