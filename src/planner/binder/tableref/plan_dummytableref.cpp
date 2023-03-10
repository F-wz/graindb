#include "graindb/planner/binder.hpp"
#include "graindb/planner/operator/logical_get.hpp"
#include "graindb/planner/tableref/bound_dummytableref.hpp"

using namespace graindb;
using namespace std;

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundEmptyTableRef &ref) {
	return make_unique<LogicalGet>(ref.bind_index);
}
