#include "graindb/planner/operator/logical_distinct.hpp"
#include "graindb/common/string_util.hpp"

using namespace graindb;
using namespace std;

using namespace graindb;
using namespace std;

string LogicalDistinct::ParamsToString() const {
	string result = LogicalOperator::ParamsToString();
	if (distinct_targets.size() > 0) {
		result += "[";
		StringUtil::Join(distinct_targets, distinct_targets.size(), ", ",
		                 [](const unique_ptr<Expression> &child) { return child->GetName(); });
		result += "]";
	}

	return result;
}
