//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/expression/bound_operator_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/planner/expression.hpp"

namespace graindb {

class BoundOperatorExpression : public Expression {
public:
	BoundOperatorExpression(ExpressionType type, TypeId return_type);

	vector<unique_ptr<Expression>> children;

public:
	string ToString() const override;

	bool Equals(const BaseExpression *other) const override;

	unique_ptr<Expression> Copy() override;
};
} // namespace graindb
