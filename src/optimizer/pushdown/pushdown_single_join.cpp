#include "graindb/optimizer/filter_pushdown.hpp"
#include "graindb/planner/operator/logical_comparison_join.hpp"

using namespace graindb;
using namespace std;

using Filter = FilterPushdown::Filter;

unique_ptr<LogicalOperator> FilterPushdown::PushdownSingleJoin(unique_ptr<LogicalOperator> op,
                                                               unordered_set<idx_t> &left_bindings,
                                                               unordered_set<idx_t> &right_bindings) {
	assert(((LogicalJoin &)*op).join_type == JoinType::SINGLE);
	FilterPushdown left_pushdown(optimizer), right_pushdown(optimizer);
	// now check the set of filters
	for (idx_t i = 0; i < filters.size(); i++) {
		auto side = JoinSide::GetJoinSide(filters[i]->bindings, left_bindings, right_bindings);
		if (side == JoinSide::LEFT) {
			// bindings match left side: push into left
			left_pushdown.filters.push_back(move(filters[i]));
			// erase the filter from the list of filters
			filters.erase(filters.begin() + i);
			i--;
		}
	}
	op->children[0] = left_pushdown.Rewrite(move(op->children[0]));
	op->children[1] = right_pushdown.Rewrite(move(op->children[1]));
	return FinishPushdown(move(op));
}
