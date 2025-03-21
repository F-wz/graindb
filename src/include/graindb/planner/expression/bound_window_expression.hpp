//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/expression/bound_window_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/expression/window_expression.hpp"
#include "graindb/planner/bound_query_node.hpp"
#include "graindb/planner/expression.hpp"

namespace graindb {
class AggregateFunction;

class BoundWindowExpression : public Expression {
public:
	BoundWindowExpression(ExpressionType type, TypeId return_type, unique_ptr<AggregateFunction> aggregate);

	//! The bound aggregate function
	unique_ptr<AggregateFunction> aggregate;
	//! The child expressions of the main window aggregate
	vector<unique_ptr<Expression>> children;
	//! The set of expressions to partition by
	vector<unique_ptr<Expression>> partitions;
	//! The set of ordering clauses
	vector<BoundOrderByNode> orders;
	//! The window boundaries
	WindowBoundary start = WindowBoundary::INVALID;
	WindowBoundary end = WindowBoundary::INVALID;

	unique_ptr<Expression> start_expr;
	unique_ptr<Expression> end_expr;
	//! Offset and default expressions for WINDOW_LEAD and WINDOW_LAG functions
	unique_ptr<Expression> offset_expr;
	unique_ptr<Expression> default_expr;

public:
	bool IsWindow() const override {
		return true;
	}
	bool IsFoldable() const override {
		return false;
	}

	string ToString() const override;

	bool Equals(const BaseExpression *other) const override;

	unique_ptr<Expression> Copy() override;
};
} // namespace graindb
