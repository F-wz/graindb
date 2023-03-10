//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/expression/bound_parameter_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/types/value.hpp"
#include "graindb/planner/expression.hpp"

namespace graindb {

class BoundParameterExpression : public Expression {
public:
	BoundParameterExpression(idx_t parameter_nr);

	SQLType sql_type;
	idx_t parameter_nr;
	Value *value;

public:
	bool IsScalar() const override;
	bool HasParameter() const override;
	bool IsFoldable() const override;

	string ToString() const override;

	bool Equals(const BaseExpression *other) const override;
	hash_t Hash() const override;

	unique_ptr<Expression> Copy() override;
};
} // namespace graindb
