//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/subquery/has_correlated_expressions.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/planner/binder.hpp"
#include "graindb/planner/logical_operator.hpp"

namespace graindb {

//! Helper class to recursively detect correlated expressions inside a single LogicalOperator
class HasCorrelatedExpressions : public LogicalOperatorVisitor {
public:
	HasCorrelatedExpressions(const vector<CorrelatedColumnInfo> &correlated);

	void VisitOperator(LogicalOperator &op) override;

	bool has_correlated_expressions;

protected:
	unique_ptr<Expression> VisitReplace(BoundColumnRefExpression &expr, unique_ptr<Expression> *expr_ptr) override;
	unique_ptr<Expression> VisitReplace(BoundSubqueryExpression &expr, unique_ptr<Expression> *expr_ptr) override;

	const vector<CorrelatedColumnInfo> &correlated_columns;
};

} // namespace graindb
