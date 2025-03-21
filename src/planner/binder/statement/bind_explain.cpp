#include "graindb/planner/binder.hpp"
#include "graindb/parser/statement/explain_statement.hpp"
#include "graindb/planner/operator/logical_explain.hpp"

using namespace graindb;
using namespace std;

BoundStatement Binder::Bind(ExplainStatement &stmt) {
	BoundStatement result;

	// bind the underlying statement
	auto plan = Bind(*stmt.stmt);
	// get the unoptimized logical plan, and create the explain statement
	auto logical_plan_unopt = plan.plan->ToString();
	auto explain = make_unique<LogicalExplain>(move(plan.plan));
	explain->logical_plan_unopt = logical_plan_unopt;

	result.plan = move(explain);
	result.names = {"explain_key", "explain_value"};
	result.types = {SQLType::VARCHAR, SQLType::VARCHAR};
	return result;
}
