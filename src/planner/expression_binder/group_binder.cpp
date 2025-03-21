#include "graindb/planner/expression_binder/group_binder.hpp"

#include "graindb/parser/expression/columnref_expression.hpp"
#include "graindb/parser/expression/constant_expression.hpp"
#include "graindb/parser/query_node/select_node.hpp"
#include "graindb/planner/expression/bound_constant_expression.hpp"

using namespace graindb;
using namespace std;

GroupBinder::GroupBinder(Binder &binder, ClientContext &context, SelectNode &node, idx_t group_index,
                         unordered_map<string, idx_t> &alias_map, unordered_map<string, idx_t> &group_alias_map)
    : ExpressionBinder(binder, context), node(node), alias_map(alias_map), group_alias_map(group_alias_map),
      group_index(group_index) {
}

BindResult GroupBinder::BindExpression(ParsedExpression &expr, idx_t depth, bool root_expression) {
	if (root_expression && depth == 0) {
		switch (expr.expression_class) {
		case ExpressionClass::COLUMN_REF:
			return BindColumnRef((ColumnRefExpression &)expr);
		case ExpressionClass::CONSTANT:
			return BindConstant((ConstantExpression &)expr);
		default:
			break;
		}
	}
	switch (expr.expression_class) {
	case ExpressionClass::DEFAULT:
		return BindResult("GROUP BY clause cannot contain DEFAULT clause");
	case ExpressionClass::WINDOW:
		return BindResult("GROUP BY clause cannot contain window functions!");
	default:
		return ExpressionBinder::BindExpression(expr, depth);
	}
}

string GroupBinder::UnsupportedAggregateMessage() {
	return "GROUP BY clause cannot contain aggregates!";
}

BindResult GroupBinder::BindSelectRef(idx_t entry) {
	if (used_aliases.find(entry) != used_aliases.end()) {
		// the alias has already been bound to before!
		// this happens if we group on the same alias twice
		// e.g. GROUP BY k, k or GROUP BY 1, 1
		// in this case, we can just replace the grouping with a constant since the second grouping has no effect
		// (the constant grouping will be optimized out later)
		return BindResult(make_unique<BoundConstantExpression>(Value(42)), SQLType::INTEGER);
	}
	if (entry >= node.select_list.size()) {
		throw BinderException("GROUP BY term out of range - should be between 1 and %d", (int)node.select_list.size());
	}
	// we replace the root expression, also replace the unbound expression
	unbound_expression = node.select_list[entry]->Copy();
	// move the expression that this refers to here and bind it
	auto select_entry = move(node.select_list[entry]);
	SQLType group_type;
	auto binding = Bind(select_entry, &group_type, false);
	// now replace the original expression in the select list with a reference to this group
	group_alias_map[to_string(entry)] = bind_index;
	node.select_list[entry] = make_unique<ColumnRefExpression>(to_string(entry));
	// insert into the set of used aliases
	used_aliases.insert(entry);
	return BindResult(move(binding), group_type);
}

BindResult GroupBinder::BindConstant(ConstantExpression &constant) {
	// constant as root expression
	if (!TypeIsIntegral(constant.value.type)) {
		// non-integral expression, we just leave the constant here.
		return ExpressionBinder::BindExpression(constant, 0);
	}
	// INTEGER constant: we use the integer as an index into the select list (e.g. GROUP BY 1)
	auto index = (idx_t)constant.value.GetValue<int64_t>();
	return BindSelectRef(index - 1);
}

BindResult GroupBinder::BindColumnRef(ColumnRefExpression &colref) {
	// columns in GROUP BY clauses:
	// FIRST refer to the original tables, and
	// THEN if no match is found refer to aliases in the SELECT list
	// THEN if no match is found, refer to outer queries

	// first try to bind to the base columns (original tables)
	auto result = ExpressionBinder::BindExpression(colref, 0);
	if (result.HasError()) {
		// failed to bind the column and the node is the root expression with depth = 0
		// check if refers to an alias in the select clause
		auto alias_name = colref.column_name;
		if (!colref.table_name.empty()) {
			// explicit table name: not an alias reference
			return result;
		}
		auto entry = alias_map.find(alias_name);
		if (entry == alias_map.end()) {
			// no matching alias found
			return result;
		}
		result = BindResult(BindSelectRef(entry->second));
		if (!result.HasError()) {
			group_alias_map[alias_name] = bind_index;
		}
	}
	return result;
}
