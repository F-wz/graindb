#include "graindb/planner/binder.hpp"
#include "graindb/planner/operator/logical_get.hpp"
#include "graindb/planner/tableref/bound_basetableref.hpp"

using namespace graindb;
using namespace std;

unique_ptr<LogicalOperator> Binder::CreatePlan(BoundBaseTableRef &ref) {
	return move(ref.get);
}
