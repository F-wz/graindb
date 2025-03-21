#include "graindb/main/relation/explain_relation.hpp"
#include "graindb/parser/statement/explain_statement.hpp"
#include "graindb/parser/statement/select_statement.hpp"
#include "graindb/parser/parsed_data/create_view_info.hpp"
#include "graindb/planner/binder.hpp"
#include "graindb/main/client_context.hpp"

namespace graindb {

ExplainRelation::ExplainRelation(shared_ptr<Relation> child_p)
    : Relation(child_p->context, RelationType::EXPLAIN_RELATION), child(move(child_p)) {
	context.TryBindRelation(*this, this->columns);
}

unique_ptr<QueryNode> ExplainRelation::GetQueryNode() {
	throw InternalException("Cannot create a query node from a ExplainRelation!");
}

BoundStatement ExplainRelation::Bind(Binder &binder) {
	auto select = make_unique<SelectStatement>();
	select->node = child->GetQueryNode();
	ExplainStatement explain(move(select));
	return binder.Bind((SQLStatement &)explain);
}

const vector<ColumnDefinition> &ExplainRelation::Columns() {
	return columns;
}

string ExplainRelation::ToString(idx_t depth) {
	string str = RenderWhitespace(depth) + "Explain\n";
	return str + child->ToString(depth + 1);
}

} // namespace graindb
