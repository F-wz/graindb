//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/expression_binder/relation_binder.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/planner/expression_binder.hpp"

namespace graindb {

//! The relation binder is a binder used to bind expressions in the relation API
class RelationBinder : public ExpressionBinder {
public:
	RelationBinder(Binder &binder, ClientContext &context, string op);

	string op;

protected:
	BindResult BindExpression(ParsedExpression &expr, idx_t depth, bool root_expression = false) override;

	string UnsupportedAggregateMessage() override;
};

} // namespace graindb
