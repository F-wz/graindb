#include "graindb/catalog/catalog_entry/table_catalog_entry.hpp"
#include "graindb/execution/operator/persistent/physical_update.hpp"
#include "graindb/execution/physical_plan_generator.hpp"
#include "graindb/planner/operator/logical_update.hpp"

using namespace graindb;
using namespace std;

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalUpdate &op) {
	assert(op.children.size() == 1);

	auto plan = CreatePlan(*op.children[0]);

	dependencies.insert(op.table);
	auto update = make_unique<PhysicalUpdate>(op, *op.table, *op.table->storage, op.columns, move(op.expressions),
	                                          move(op.bound_defaults));
	update->is_index_update = op.is_index_update;
	update->children.push_back(move(plan));
	return move(update);
}
