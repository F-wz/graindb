#include "graindb/parser/expression/columnref_expression.hpp"
#include "graindb/planner/binder.hpp"
#include "graindb/planner/expression/bound_columnref_expression.hpp"
#include "graindb/planner/expression_binder.hpp"
#include "graindb/common/string_util.hpp"

using namespace graindb;
using namespace std;

BindResult ExpressionBinder::BindExpression(ColumnRefExpression &colref, idx_t depth) {
	assert(!colref.column_name.empty());
	// individual column reference
	// resolve to either a base table or a subquery expression
	if (colref.table_name.empty()) {
		// no table name: find a binding that contains this
		colref.table_name = binder.bind_context.GetMatchingBinding(colref.column_name);
		if (colref.table_name.empty()) {
			return BindResult(
			    StringUtil::Format("Referenced column \"%s\" not found in FROM clause!", colref.column_name.c_str()));
		}
	}
	BindResult result = binder.bind_context.BindColumn(colref, depth);
	if (!result.HasError()) {
		bound_columns = true;
	}
	return result;
}
