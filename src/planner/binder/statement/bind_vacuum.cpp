#include "graindb/planner/binder.hpp"
#include "graindb/parser/statement/vacuum_statement.hpp"
#include "graindb/planner/operator/logical_simple.hpp"

using namespace std;

namespace graindb {

BoundStatement Binder::Bind(VacuumStatement &stmt) {
	BoundStatement result;
	result.names = {"Success"};
	result.types = {SQLType::BOOLEAN};
	result.plan = make_unique<LogicalSimple>(LogicalOperatorType::VACUUM, move(stmt.info));
	return result;
}

} // namespace graindb
