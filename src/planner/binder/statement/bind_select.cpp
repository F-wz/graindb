#include "graindb/parser/statement/select_statement.hpp"
#include "graindb/planner/binder.hpp"
#include "graindb/planner/bound_query_node.hpp"

using namespace graindb;
using namespace std;

BoundStatement Binder::Bind(SelectStatement &stmt) {
	// first we visit the set of CTEs and add them to the bind context
	for (auto &cte_it : stmt.cte_map) {
		AddCTE(cte_it.first, cte_it.second.get());
	}
	// now visit the root node of the select statement
	return Bind(*stmt.node);
}
