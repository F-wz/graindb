#include "graindb/parser/statement/explain_statement.hpp"
#include "graindb/parser/transformer.hpp"

using namespace graindb;
using namespace std;

unique_ptr<ExplainStatement> Transformer::TransformExplain(PGNode *node) {
	PGExplainStmt *stmt = reinterpret_cast<PGExplainStmt *>(node);
	assert(stmt);
	return make_unique<ExplainStatement>(TransformStatement(stmt->query));
}
