#include "graindb/parser/statement/delete_statement.hpp"
#include "graindb/planner/binder.hpp"
#include "graindb/planner/expression_binder/where_binder.hpp"
#include "graindb/planner/operator/logical_delete.hpp"
#include "graindb/planner/operator/logical_filter.hpp"
#include "graindb/planner/operator/logical_get.hpp"
#include "graindb/planner/bound_tableref.hpp"

using namespace graindb;
using namespace std;

BoundStatement Binder::Bind(DeleteStatement &stmt) {
	BoundStatement result;

	// visit the table reference
	auto bound_table = Bind(*stmt.table);
	if (bound_table->type != TableReferenceType::BASE_TABLE) {
		throw BinderException("Can only delete from base table!");
	}
	auto root = CreatePlan(*bound_table);
	auto &get = (LogicalGet &)*root;
	assert(root->type == LogicalOperatorType::GET && get.table);

	if (!get.table->temporary) {
		// delete from persistent table: not read only!
		this->read_only = false;
	}
	// project any additional columns required for the condition
	unique_ptr<Expression> condition;
	if (stmt.condition) {
		WhereBinder binder(*this, context);
		condition = binder.Bind(stmt.condition);

		PlanSubqueries(&condition, &root);
		auto filter = make_unique<LogicalFilter>(move(condition));
		filter->AddChild(move(root));
		root = move(filter);
	}
	// create the delete node
	auto del = make_unique<LogicalDelete>(get.table);
	del->AddChild(move(root));

	// set up the delete expression
	del->expressions.push_back(
	    make_unique<BoundColumnRefExpression>(TypeId::INT64, ColumnBinding(get.table_index, get.column_ids.size())));
	get.column_ids.push_back(COLUMN_IDENTIFIER_ROW_ID);

	result.plan = move(del);
	result.names = {"Count"};
	result.types = {SQLType::BIGINT};
	return result;
}
