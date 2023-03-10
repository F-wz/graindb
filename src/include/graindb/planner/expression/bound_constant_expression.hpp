//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/expression/bound_constant_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/types/value.hpp"
#include "graindb/planner/expression.hpp"

namespace graindb {

class BoundConstantExpression : public Expression {
public:
	BoundConstantExpression(Value value);

	Value value;

public:
	string ToString() const override;

	bool Equals(const BaseExpression *other) const override;
	hash_t Hash() const override;

	unique_ptr<Expression> Copy() override;
};
} // namespace graindb
