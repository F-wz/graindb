#include "graindb/main/relation/update_relation.hpp"
#include "graindb/parser/statement/update_statement.hpp"
#include "graindb/planner/binder.hpp"
#include "graindb/main/client_context.hpp"
#include "graindb/parser/tableref/basetableref.hpp"

namespace graindb {

UpdateRelation::UpdateRelation(ClientContext &context, unique_ptr<ParsedExpression> condition_p, string schema_name_p,
                               string table_name_p, vector<string> update_columns_p,
                               vector<unique_ptr<ParsedExpression>> expressions_p)
    : Relation(context, RelationType::UPDATE_RELATION), condition(move(condition_p)), schema_name(move(schema_name_p)),
      table_name(move(table_name_p)), update_columns(move(update_columns_p)), expressions(move(expressions_p)) {
	assert(update_columns.size() == expressions.size());
	context.TryBindRelation(*this, this->columns);
}

unique_ptr<QueryNode> UpdateRelation::GetQueryNode() {
	throw InternalException("Cannot create a query node from a UpdateRelation!");
}

BoundStatement UpdateRelation::Bind(Binder &binder) {
	auto basetable = make_unique<BaseTableRef>();
	basetable->schema_name = schema_name;
	basetable->table_name = table_name;

	UpdateStatement stmt;
	stmt.condition = condition ? condition->Copy() : nullptr;
	stmt.table = move(basetable);
	stmt.columns = update_columns;
	for (auto &expr : expressions) {
		stmt.expressions.push_back(expr->Copy());
	}
	return binder.Bind((SQLStatement &)stmt);
}

const vector<ColumnDefinition> &UpdateRelation::Columns() {
	return columns;
}

string UpdateRelation::ToString(idx_t depth) {
	string str = RenderWhitespace(depth) + "UPDATE " + table_name + " SET\n";
	for (idx_t i = 0; i < expressions.size(); i++) {
		str += update_columns[i] + " = " + expressions[i]->ToString() + "\n";
	}
	if (condition) {
		str += "WHERE " + condition->ToString() + "\n";
	}
	return str;
}

} // namespace graindb
