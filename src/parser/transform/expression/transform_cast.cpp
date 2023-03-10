#include "graindb/common/limits.hpp"
#include "graindb/parser/expression/cast_expression.hpp"
#include "graindb/parser/expression/constant_expression.hpp"
#include "graindb/parser/transformer.hpp"

using namespace graindb;
using namespace std;

unique_ptr<ParsedExpression> Transformer::TransformTypeCast(PGTypeCast *root) {
	if (!root) {
		return nullptr;
	}
	// get the type to cast to
	auto type_name = root->typeName;
	SQLType target_type = TransformTypeName(type_name);

	// transform the expression node
	auto expression = TransformExpression(root->arg);
	// now create a cast operation
	return make_unique<CastExpression>(target_type, move(expression));
}
