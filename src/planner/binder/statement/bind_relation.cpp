#include "graindb/catalog/catalog.hpp"
#include "graindb/parser/expression/constant_expression.hpp"
#include "graindb/parser/statement/insert_statement.hpp"
#include "graindb/parser/query_node/select_node.hpp"
#include "graindb/planner/binder.hpp"
#include "graindb/parser/statement/relation_statement.hpp"

using namespace std;

namespace graindb {

BoundStatement Binder::Bind(RelationStatement &stmt) {
	return stmt.relation->Bind(*this);
}

} // namespace graindb
