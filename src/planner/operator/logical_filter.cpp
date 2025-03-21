#include "graindb/planner/operator/logical_filter.hpp"

#include "graindb/planner/expression/bound_conjunction_expression.hpp"

using namespace graindb;
using namespace std;

LogicalFilter::LogicalFilter(unique_ptr<Expression> expression) : LogicalOperator(LogicalOperatorType::FILTER) {
	expressions.push_back(move(expression));
	SplitPredicates(expressions);
}

LogicalFilter::LogicalFilter() : LogicalOperator(LogicalOperatorType::FILTER) {
}

void LogicalFilter::ResolveTypes() {
	types = MapTypes(children[0]->types, projection_map);
}

vector<ColumnBinding> LogicalFilter::GetColumnBindings() {
	return MapBindings(children[0]->GetColumnBindings(), projection_map);
}

ColumnBinding LogicalFilter::PushdownColumnBinding(ColumnBinding &binding) {
	auto child_binding = children[0]->PushdownColumnBinding(binding);
	if (child_binding.column_index != INVALID_INDEX) {
		if (projection_map.size() == 0) {
			return child_binding;
		}
		projection_map.push_back(child_binding.column_index);
		return child_binding;
	}
	return ColumnBinding(binding.table_index, INVALID_INDEX, INVALID_INDEX, binding.table);
}

// Split the predicates separated by AND statements
// These are the predicates that are safe to push down because all of them MUST
// be true
bool LogicalFilter::SplitPredicates(vector<unique_ptr<Expression>> &expressions) {
	bool found_conjunction = false;
	for (idx_t i = 0; i < expressions.size(); i++) {
		if (expressions[i]->type == ExpressionType::CONJUNCTION_AND) {
			auto &conjunction = (BoundConjunctionExpression &)*expressions[i];
			found_conjunction = true;
			// AND expression, append the other children
			for (idx_t k = 1; k < conjunction.children.size(); k++) {
				expressions.push_back(move(conjunction.children[k]));
			}
			// replace this expression with the first child of the conjunction
			expressions[i] = move(conjunction.children[0]);
			// we move back by one so the right child is checked again
			// in case it is an AND expression as well
			i--;
		}
	}
	return found_conjunction;
}
