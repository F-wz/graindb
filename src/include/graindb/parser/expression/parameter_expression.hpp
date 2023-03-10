//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/expression/parameter_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_expression.hpp"

namespace graindb {
class ParameterExpression : public ParsedExpression {
public:
	ParameterExpression();

	idx_t parameter_nr;

public:
	bool IsScalar() const override {
		return true;
	}
	bool HasParameter() const override {
		return true;
	}

	string ToString() const override;

	unique_ptr<ParsedExpression> Copy() const override;
	hash_t Hash() const override;

	void Serialize(Serializer &serializer) override;
	static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, Deserializer &source);
};
} // namespace graindb
