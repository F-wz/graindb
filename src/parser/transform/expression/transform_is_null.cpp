#include "graindb/common/exception.hpp"
#include "graindb/parser/expression/operator_expression.hpp"
#include "graindb/parser/transformer.hpp"

using namespace graindb;
using namespace std;

unique_ptr<ParsedExpression> Transformer::TransformNullTest(PGNullTest *root) {
	assert(root);
	auto arg = TransformExpression(reinterpret_cast<PGNode *>(root->arg));
	if (root->argisrow) {
		throw NotImplementedException("IS NULL argisrow");
	}
	ExpressionType expr_type =
	    (root->nulltesttype == PG_IS_NULL) ? ExpressionType::OPERATOR_IS_NULL : ExpressionType::OPERATOR_IS_NOT_NULL;

	return unique_ptr<ParsedExpression>(new OperatorExpression(expr_type, move(arg)));
}
