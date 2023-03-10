#include "graindb/planner/binder.hpp"
#include "graindb/planner/operator/logical_table_function.hpp"
#include "graindb/planner/tableref/bound_table_function.hpp"

using namespace graindb;
using namespace std;

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundTableFunction &ref) {
	return make_unique<LogicalTableFunction>(ref.function, ref.bind_index, move(ref.bind_data), move(ref.parameters),
	                                         ref.return_types, ref.names);
}
