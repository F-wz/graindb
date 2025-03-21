#include "graindb/execution/operator/aggregate/physical_hash_aggregate.hpp"
#include "graindb/execution/operator/aggregate/physical_simple_aggregate.hpp"
#include "graindb/execution/physical_plan_generator.hpp"
#include "graindb/catalog/catalog_entry/aggregate_function_catalog_entry.hpp"
#include "graindb/planner/expression/bound_aggregate_expression.hpp"
#include "graindb/planner/operator/logical_aggregate.hpp"

using namespace graindb;
using namespace std;

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalAggregate &op) {
	assert(op.children.size() == 1);

	auto plan = CreatePlan(*op.children[0]);
	if (op.groups.size() == 0) {
		// no groups, check if we can use a simple aggregation
		// special case: aggregate entire columns together
		bool use_simple_aggregation = true;
		for (idx_t i = 0; i < op.expressions.size(); i++) {
			auto &aggregate = (BoundAggregateExpression &)*op.expressions[i];
			if (!aggregate.function.simple_update || aggregate.distinct) {
				// unsupported aggregate for simple aggregation: use hash aggregation
				use_simple_aggregation = false;
				break;
			}
		}
		if (use_simple_aggregation) {
			auto groupby = make_unique<PhysicalSimpleAggregate>(op.types, move(op.expressions));
			groupby->children.push_back(move(plan));
			return move(groupby);
		} else {
			auto groupby = make_unique<PhysicalHashAggregate>(op.types, move(op.expressions));
			groupby->children.push_back(move(plan));
			return move(groupby);
		}
	} else {
		// groups! create a GROUP BY aggregator
		auto groupby = make_unique<PhysicalHashAggregate>(op.types, move(op.expressions), move(op.groups));
		groupby->children.push_back(move(plan));
		return move(groupby);
	}
}
