#include "graindb/function/table/sqlite_functions.hpp"
#include "graindb/parser/parsed_data/create_view_info.hpp"
#include "graindb/parser/query_node/select_node.hpp"
#include "graindb/parser/expression/star_expression.hpp"
#include "graindb/parser/tableref/table_function_ref.hpp"
#include "graindb/parser/expression/function_expression.hpp"
#include "graindb/catalog/catalog.hpp"

using namespace std;

namespace graindb {

void BuiltinFunctions::RegisterSQLiteFunctions() {
	PragmaCollations::RegisterFunction(*this);
	PragmaTableInfo::RegisterFunction(*this);
	SQLiteMaster::RegisterFunction(*this);

	CreateViewInfo info;
	info.schema = DEFAULT_SCHEMA;
	info.view_name = "sqlite_master";
	info.on_conflict = OnCreateConflict::REPLACE;

	auto select = make_unique<SelectNode>();
	select->select_list.push_back(make_unique<StarExpression>());
	vector<unique_ptr<ParsedExpression>> children;

	auto function = make_unique<FunctionExpression>(DEFAULT_SCHEMA, "sqlite_master", children);
	auto function_expr = make_unique<TableFunctionRef>();
	function_expr->function = move(function);
	select->from_table = move(function_expr);
	info.query = move(select);
	//	catalog.CreateView(transaction, &info);
}

} // namespace graindb
