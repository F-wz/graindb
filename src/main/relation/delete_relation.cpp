#include "graindb/main/relation/delete_relation.hpp"
#include "graindb/parser/statement/delete_statement.hpp"
#include "graindb/planner/binder.hpp"
#include "graindb/main/client_context.hpp"
#include "graindb/parser/tableref/basetableref.hpp"

namespace graindb {

DeleteRelation::DeleteRelation(ClientContext &context, unique_ptr<ParsedExpression> condition_p, string schema_name_p,
                               string table_name_p)
    : Relation(context, RelationType::DELETE_RELATION), condition(move(condition_p)), schema_name(move(schema_name_p)),
      table_name(move(table_name_p)) {
	context.TryBindRelation(*this, this->columns);
}

unique_ptr<QueryNode> DeleteRelation::GetQueryNode() {
	throw InternalException("Cannot create a query node from a DeleteRelation!");
}

BoundStatement DeleteRelation::Bind(Binder &binder) {
	auto basetable = make_unique<BaseTableRef>();
	basetable->schema_name = schema_name;
	basetable->table_name = table_name;

	DeleteStatement stmt;
	stmt.condition = condition ? condition->Copy() : nullptr;
	stmt.table = move(basetable);
	return binder.Bind((SQLStatement &)stmt);
}

const vector<ColumnDefinition> &DeleteRelation::Columns() {
	return columns;
}

string DeleteRelation::ToString(idx_t depth) {
	string str = RenderWhitespace(depth) + "DELETE FROM " + table_name;
	if (condition) {
		str += " WHERE " + condition->ToString();
	}
	return str;
}

} // namespace graindb
