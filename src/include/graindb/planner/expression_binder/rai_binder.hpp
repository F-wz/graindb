//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/expression_binder/rai_binder.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/planner/expression_binder.hpp"

namespace graindb {
class BoundColumnRefExpression;

class RAIBinder : public ExpressionBinder {
public:
	RAIBinder(Binder &binder, ClientContext &context);

protected:
	BindResult BindExpression(ParsedExpression &expr, idx_t depth, bool root_expression = false) override;

	string UnsupportedAggregateMessage() override;
};
} // namespace graindb
