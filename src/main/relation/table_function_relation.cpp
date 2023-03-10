#include "graindb/main/relation/table_function_relation.hpp"
#include "graindb/parser/tableref/basetableref.hpp"
#include "graindb/parser/query_node/select_node.hpp"
#include "graindb/parser/expression/star_expression.hpp"
#include "graindb/parser/tableref/table_function_ref.hpp"
#include "graindb/parser/expression/constant_expression.hpp"
#include "graindb/parser/expression/function_expression.hpp"
#include "graindb/main/client_context.hpp"
#include "graindb/parser/parser.hpp"

namespace graindb {

TableFunctionRelation::TableFunctionRelation(ClientContext &context, string name_p, vector<Value> parameters_p)
    : Relation(context, RelationType::TABLE_FUNCTION_RELATION), name(move(name_p)), parameters(move(parameters_p)) {
	context.TryBindRelation(*this, this->columns);
}

unique_ptr<QueryNode> TableFunctionRelation::GetQueryNode() {
	auto result = make_unique<SelectNode>();
	result->select_list.push_back(make_unique<StarExpression>());
	result->from_table = GetTableRef();
	return move(result);
}

unique_ptr<TableRef> TableFunctionRelation::GetTableRef() {
	vector<unique_ptr<ParsedExpression>> children;
	for (auto &parameter : parameters) {
		children.push_back(make_unique<ConstantExpression>(parameter.GetSQLType(), parameter));
	}

	auto table_function = make_unique<TableFunctionRef>();
	auto function = make_unique<FunctionExpression>(name, children);
	table_function->function = move(function);
	return move(table_function);
}

string TableFunctionRelation::GetAlias() {
	return name;
}

const vector<ColumnDefinition> &TableFunctionRelation::Columns() {
	return columns;
}

string TableFunctionRelation::ToString(idx_t depth) {
	string function_call = name + "(";
	for (idx_t i = 0; i < parameters.size(); i++) {
		if (i > 0) {
			function_call += ", ";
		}
		function_call += parameters[i].ToString();
	}
	function_call += ")";
	return RenderWhitespace(depth) + function_call;
}

} // namespace graindb
