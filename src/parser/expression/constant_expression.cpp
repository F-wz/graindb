#include "graindb/parser/expression/constant_expression.hpp"

#include "graindb/common/exception.hpp"
#include "graindb/common/types/hash.hpp"
#include "graindb/common/value_operations/value_operations.hpp"

using namespace graindb;
using namespace std;

ConstantExpression::ConstantExpression(SQLType sql_type, Value val)
    : ParsedExpression(ExpressionType::VALUE_CONSTANT, ExpressionClass::CONSTANT), value(val), sql_type(sql_type) {
}

string ConstantExpression::ToString() const {
	return value.ToString();
}

bool ConstantExpression::Equals(const ConstantExpression *a, const ConstantExpression *b) {
	return a->value == b->value;
}

hash_t ConstantExpression::Hash() const {
	return ParsedExpression::Hash();
}

unique_ptr<ParsedExpression> ConstantExpression::Copy() const {
	auto copy = make_unique<ConstantExpression>(sql_type, value);
	copy->CopyProperties(*this);
	return move(copy);
}

void ConstantExpression::Serialize(Serializer &serializer) {
	ParsedExpression::Serialize(serializer);
	value.Serialize(serializer);
	sql_type.Serialize(serializer);
}

unique_ptr<ParsedExpression> ConstantExpression::Deserialize(ExpressionType type, Deserializer &source) {
	Value value = Value::Deserialize(source);
	auto sql_type = SQLType::Deserialize(source);
	return make_unique<ConstantExpression>(sql_type, value);
}
