//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/expression/bound_subquery_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/enums/subquery_type.hpp"
#include "graindb/planner/binder.hpp"
#include "graindb/planner/bound_query_node.hpp"
#include "graindb/planner/expression.hpp"

namespace graindb {

class BoundSubqueryExpression : public Expression {
public:
	BoundSubqueryExpression(TypeId return_type);

	bool IsCorrelated() {
		return binder->correlated_columns.size() > 0;
	}

	//! The binder used to bind the subquery node
	unique_ptr<Binder> binder;
	//! The bound subquery node
	unique_ptr<BoundQueryNode> subquery;
	//! The subquery type
	SubqueryType subquery_type;
	//! the child expression to compare with (in case of IN, ANY, ALL operators)
	unique_ptr<Expression> child;
	//! The comparison type of the child expression with the subquery (in case of ANY, ALL operators)
	ExpressionType comparison_type;
	//! The SQLType of the subquery result. Only used for ANY expressions.
	SQLType child_type;
	//! The target SQLType of the subquery result (i.e. to which type it should be casted, if child_type <>
	//! child_target). Only used for ANY expressions.
	SQLType child_target;

public:
	bool HasSubquery() const override {
		return true;
	}
	bool IsScalar() const override {
		return false;
	}
	bool IsFoldable() const override {
		return false;
	}

	string ToString() const override;

	bool Equals(const BaseExpression *other) const override;

	unique_ptr<Expression> Copy() override;
};
} // namespace graindb
