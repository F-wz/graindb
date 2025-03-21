#include "graindb/main/relation/filter_relation.hpp"
#include "graindb/main/client_context.hpp"
#include "graindb/parser/query_node/select_node.hpp"
#include "graindb/parser/query_node/set_operation_node.hpp"
#include "graindb/parser/expression/conjunction_expression.hpp"
#include "graindb/parser/expression/star_expression.hpp"

namespace graindb {

FilterRelation::FilterRelation(shared_ptr<Relation> child_p, unique_ptr<ParsedExpression> condition_p)
    : Relation(child_p->context, RelationType::FILTER_RELATION), condition(move(condition_p)), child(move(child_p)) {
	vector<ColumnDefinition> dummy_columns;
	context.TryBindRelation(*this, dummy_columns);
}

unique_ptr<QueryNode> FilterRelation::GetQueryNode() {
	auto child_ptr = child.get();
	while (child_ptr->InheritsColumnBindings()) {
		child_ptr = child_ptr->ChildRelation();
	}
	if (child_ptr->type == RelationType::JOIN_RELATION) {
		// child node is a join: push filter into WHERE clause of select node
		auto child_node = child->GetQueryNode();
		assert(child_node->type == QueryNodeType::SELECT_NODE);
		auto &select_node = (SelectNode &)*child_node;
		if (!select_node.where_clause) {
			select_node.where_clause = condition->Copy();
		} else {
			select_node.where_clause = make_unique<ConjunctionExpression>(
			    ExpressionType::CONJUNCTION_AND, move(select_node.where_clause), condition->Copy());
		}
		return child_node;
	} else {
		auto result = make_unique<SelectNode>();
		result->select_list.push_back(make_unique<StarExpression>());
		result->from_table = child->GetTableRef();
		result->where_clause = condition->Copy();
		return move(result);
	}
}

string FilterRelation::GetAlias() {
	return child->GetAlias();
}

const vector<ColumnDefinition> &FilterRelation::Columns() {
	return child->Columns();
}

string FilterRelation::ToString(idx_t depth) {
	string str = RenderWhitespace(depth) + "Filter [" + condition->ToString() + "]\n";
	return str + child->ToString(depth + 1);
}

} // namespace graindb
