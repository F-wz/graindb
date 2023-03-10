#include "graindb/parser/expression/parameter_expression.hpp"

#include "graindb/common/exception.hpp"
#include "graindb/common/serializer.hpp"
#include "graindb/common/types/hash.hpp"

using namespace graindb;
using namespace std;

ParameterExpression::ParameterExpression()
    : ParsedExpression(ExpressionType::VALUE_PARAMETER, ExpressionClass::PARAMETER), parameter_nr(0) {
}

string ParameterExpression::ToString() const {
	return "$" + std::to_string(parameter_nr);
}

unique_ptr<ParsedExpression> ParameterExpression::Copy() const {
	auto copy = make_unique<ParameterExpression>();
	copy->CopyProperties(*this);
	return move(copy);
}

hash_t ParameterExpression::Hash() const {
	hash_t result = ParsedExpression::Hash();
	return CombineHash(graindb::Hash(parameter_nr), result);
}

void ParameterExpression::Serialize(Serializer &serializer) {
	ParsedExpression::Serialize(serializer);
	serializer.Write<idx_t>(parameter_nr);
}

unique_ptr<ParsedExpression> ParameterExpression::Deserialize(ExpressionType type, Deserializer &source) {
	auto expression = make_unique<ParameterExpression>();
	expression->parameter_nr = source.Read<idx_t>();
	return move(expression);
}
