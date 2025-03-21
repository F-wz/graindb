#include "graindb/main/relation/view_relation.hpp"
#include "graindb/parser/tableref/basetableref.hpp"
#include "graindb/parser/query_node/select_node.hpp"
#include "graindb/parser/expression/star_expression.hpp"
#include "graindb/main/client_context.hpp"
#include "graindb/parser/parser.hpp"

namespace graindb {

ViewRelation::ViewRelation(ClientContext &context, string schema_name_p, string view_name_p)
    : Relation(context, RelationType::VIEW_RELATION), schema_name(move(schema_name_p)), view_name(move(view_name_p)) {
	context.TryBindRelation(*this, this->columns);
}

unique_ptr<QueryNode> ViewRelation::GetQueryNode() {
	auto result = make_unique<SelectNode>();
	result->select_list.push_back(make_unique<StarExpression>());
	result->from_table = GetTableRef();
	return move(result);
}

unique_ptr<TableRef> ViewRelation::GetTableRef() {
	auto table_ref = make_unique<BaseTableRef>();
	table_ref->schema_name = schema_name;
	table_ref->table_name = view_name;
	return move(table_ref);
}

string ViewRelation::GetAlias() {
	return view_name;
}

const vector<ColumnDefinition> &ViewRelation::Columns() {
	return columns;
}

string ViewRelation::ToString(idx_t depth) {
	return RenderWhitespace(depth) + "View [" + view_name + "]";
}

} // namespace graindb
