#include "graindb/main/relation/create_table_relation.hpp"
#include "graindb/parser/statement/create_statement.hpp"
#include "graindb/parser/statement/select_statement.hpp"
#include "graindb/parser/parsed_data/create_table_info.hpp"
#include "graindb/planner/binder.hpp"
#include "graindb/main/client_context.hpp"

namespace graindb {

CreateTableRelation::CreateTableRelation(shared_ptr<Relation> child_p, string schema_name, string table_name)
    : Relation(child_p->context, RelationType::CREATE_TABLE_RELATION), child(move(child_p)), schema_name(move(schema_name)),
      table_name(move(table_name)) {
	context.TryBindRelation(*this, this->columns);
}

unique_ptr<QueryNode> CreateTableRelation::GetQueryNode() {
	throw InternalException("Cannot create a query node from a CreateTableRelation!");
}

BoundStatement CreateTableRelation::Bind(Binder &binder) {
	auto select = make_unique<SelectStatement>();
	select->node = child->GetQueryNode();

	CreateStatement stmt;
	auto info = make_unique<CreateTableInfo>();
	info->schema = schema_name;
	info->table = table_name;
	info->query = move(select);
	info->on_conflict = OnCreateConflict::ERROR;
	stmt.info = move(info);
	return binder.Bind((SQLStatement &)stmt);
}

const vector<ColumnDefinition> &CreateTableRelation::Columns() {
	return columns;
}

string CreateTableRelation::ToString(idx_t depth) {
	string str = RenderWhitespace(depth) + "Create View\n";
	return str + child->ToString(depth + 1);
}

} // namespace graindb
