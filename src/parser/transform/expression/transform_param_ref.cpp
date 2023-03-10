#include "graindb/parser/expression/parameter_expression.hpp"
#include "graindb/parser/transformer.hpp"

using namespace graindb;
using namespace std;

unique_ptr<ParsedExpression> Transformer::TransformParamRef(PGParamRef *node) {
	if (!node) {
		return nullptr;
	}
	auto expr = make_unique<ParameterExpression>();
	if (node->number == 0) {
		expr->parameter_nr = ParamCount() + 1;
	} else {
		expr->parameter_nr = node->number;
	}
	SetParamCount(max(ParamCount(), expr->parameter_nr));
	return move(expr);
}
