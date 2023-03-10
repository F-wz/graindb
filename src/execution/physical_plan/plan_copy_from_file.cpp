#include "graindb/execution/physical_plan_generator.hpp"
#include "graindb/execution/operator/persistent/physical_copy_from_file.hpp"
#include "graindb/planner/operator/logical_copy_from_file.hpp"

using namespace graindb;
using namespace std;

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalCopyFromFile &op) {
	// COPY from file into a table
	return make_unique<PhysicalCopyFromFile>(op, op.sql_types, move(op.info));
}
