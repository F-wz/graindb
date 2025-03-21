//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/expression/bound_reference_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/planner/column_binding.hpp"
#include "graindb/planner/expression.hpp"

namespace graindb {

//! A BoundReferenceExpression represents a physical index into a DataChunk
class BoundReferenceExpression : public Expression {
public:
	BoundReferenceExpression(string alias, TypeId type, idx_t index);
	BoundReferenceExpression(TypeId type, idx_t index);

	//! Index used to access data in the chunks
	idx_t index;

public:
	bool IsScalar() const override {
		return false;
	}
	bool IsFoldable() const override {
		return false;
	}

	string ToString() const override;

	hash_t Hash() const override;
	bool Equals(const BaseExpression *other) const override;

	unique_ptr<Expression> Copy() override;
};
} // namespace graindb
