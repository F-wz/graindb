#include "graindb/optimizer/filter_pushdown.hpp"

#include "graindb/optimizer/filter_combiner.hpp"
#include "graindb/planner/operator/logical_filter.hpp"
#include "graindb/planner/operator/logical_join.hpp"

using namespace graindb;
using namespace std;

using Filter = FilterPushdown::Filter;

unique_ptr<LogicalOperator> FilterPushdown::Rewrite(unique_ptr<LogicalOperator> op) {
	assert(!combiner.HasFilters());
	switch (op->type) {
	case LogicalOperatorType::AGGREGATE_AND_GROUP_BY:
		return PushdownAggregate(move(op));
	case LogicalOperatorType::FILTER:
		return PushdownFilter(move(op));
	case LogicalOperatorType::CROSS_PRODUCT:
		return PushdownCrossProduct(move(op));
	case LogicalOperatorType::COMPARISON_JOIN:
	case LogicalOperatorType::ANY_JOIN:
	case LogicalOperatorType::DELIM_JOIN:
		return PushdownJoin(move(op));
	case LogicalOperatorType::PROJECTION:
		return PushdownProjection(move(op));
	case LogicalOperatorType::INTERSECT:
	case LogicalOperatorType::EXCEPT:
	case LogicalOperatorType::UNION:
		return PushdownSetOperation(move(op));
	case LogicalOperatorType::DISTINCT:
	case LogicalOperatorType::ORDER_BY:
	case LogicalOperatorType::CREATE_RAI: {
		// we can just push directly through these operations without any rewriting
		op->children[0] = Rewrite(move(op->children[0]));
		return op;
	}
	case LogicalOperatorType::GET:
		return PushdownGet(move(op));
	default:
		return FinishPushdown(move(op));
	}
}

unique_ptr<LogicalOperator> FilterPushdown::PushdownJoin(unique_ptr<LogicalOperator> op) {
	assert(op->type == LogicalOperatorType::COMPARISON_JOIN || op->type == LogicalOperatorType::ANY_JOIN ||
	       op->type == LogicalOperatorType::DELIM_JOIN);
	auto &join = (LogicalJoin &)*op;
	unordered_set<idx_t> left_bindings, right_bindings;
	LogicalJoin::GetTableReferences(*op->children[0], left_bindings);
	LogicalJoin::GetTableReferences(*op->children[1], right_bindings);

	switch (join.join_type) {
	case JoinType::INNER:
		return PushdownInnerJoin(move(op), left_bindings, right_bindings);
	case JoinType::LEFT:
		return PushdownLeftJoin(move(op), left_bindings, right_bindings);
	case JoinType::MARK:
		return PushdownMarkJoin(move(op), left_bindings, right_bindings);
	case JoinType::SINGLE:
		return PushdownSingleJoin(move(op), left_bindings, right_bindings);
	default:
		// unsupported join type: stop pushing down
		return FinishPushdown(move(op));
	}
}

void FilterPushdown::PushFilters() {
	for (auto &f : filters) {
		auto result = combiner.AddFilter(move(f->filter));
		assert(result == FilterResult::SUCCESS);
	}
	filters.clear();
}

FilterResult FilterPushdown::AddFilter(unique_ptr<Expression> expr) {
	PushFilters();
	// split up the filters by AND predicate
	vector<unique_ptr<Expression>> expressions;
	expressions.push_back(move(expr));
	LogicalFilter::SplitPredicates(expressions);
	// push the filters into the combiner
	for (auto &expr : expressions) {
		if (combiner.AddFilter(move(expr)) == FilterResult::UNSATISFIABLE) {
			return FilterResult::UNSATISFIABLE;
		}
	}
	return FilterResult::SUCCESS;
}

void FilterPushdown::GenerateFilters() {
	if (filters.size() > 0) {
		assert(!combiner.HasFilters());
		return;
	}
	combiner.GenerateFilters([&](unique_ptr<Expression> filter) {
		auto f = make_unique<Filter>();
		f->filter = move(filter);
		f->ExtractBindings();
		filters.push_back(move(f));
	});
}

unique_ptr<LogicalOperator> FilterPushdown::FinishPushdown(unique_ptr<LogicalOperator> op) {
	// unhandled type, first perform filter pushdown in its children
	for (idx_t i = 0; i < op->children.size(); i++) {
		FilterPushdown pushdown(optimizer);
		op->children[i] = pushdown.Rewrite(move(op->children[i]));
	}
	// now push any existing filters
	if (filters.size() == 0) {
		// no filters to push
		return op;
	}
	auto filter = make_unique<LogicalFilter>();
	for (auto &f : filters) {
		filter->expressions.push_back(move(f->filter));
	}
	filter->children.push_back(move(op));
	return move(filter);
}

void FilterPushdown::Filter::ExtractBindings() {
	bindings.clear();
	LogicalJoin::GetExpressionBindings(*filter, bindings);
}
