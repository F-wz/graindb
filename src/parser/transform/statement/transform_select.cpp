#include "graindb/parser/statement/select_statement.hpp"
#include "graindb/parser/transformer.hpp"
#include "graindb/common/string_util.hpp"

using namespace graindb;
using namespace std;

unique_ptr<SelectStatement> Transformer::TransformSelect(PGNode *node) {
	auto stmt = reinterpret_cast<PGSelectStmt *>(node);
	auto result = make_unique<SelectStatement>();

	// may contain windows so second
	if (stmt->withClause) {
		TransformCTE(reinterpret_cast<PGWithClause *>(stmt->withClause), *result);
	}

	result->node = TransformSelectNode(stmt);
	return result;
}
