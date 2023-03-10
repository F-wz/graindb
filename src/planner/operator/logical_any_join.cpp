#include "graindb/planner/operator/logical_any_join.hpp"

using namespace graindb;
using namespace std;

LogicalAnyJoin::LogicalAnyJoin(JoinType type) : LogicalJoin(type, LogicalOperatorType::ANY_JOIN) {
}

string LogicalAnyJoin::ParamsToString() const {
	return "[" + condition->ToString() + "]";
}
