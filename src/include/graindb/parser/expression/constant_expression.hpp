//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/expression/constant_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/types/value.hpp"
#include "graindb/parser/parsed_expression.hpp"

namespace graindb {
//! ConstantExpression represents a constant value in the query
class ConstantExpression : public ParsedExpression {
public:
	ConstantExpression(SQLType sql_type, Value val);

	//! The constant value referenced
	Value value;
	//! The SQL Type of the value
	SQLType sql_type;

public:
	string ToString() const override;

	static bool Equals(const ConstantExpression *a, const ConstantExpression *b);
	hash_t Hash() const override;

	unique_ptr<ParsedExpression> Copy() const override;

	void Serialize(Serializer &serializer) override;
	static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, Deserializer &source);
};
} // namespace graindb
