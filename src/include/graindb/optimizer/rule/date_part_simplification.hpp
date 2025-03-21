//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/optimizer/rule/date_part_simplification.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/optimizer/rule.hpp"

namespace graindb {

// The DatePart Simplification rule rewrites date_part with a constant specifier into a specialized funciton (e.g.
// date_part('year', x) => year(x))
class DatePartSimplificationRule : public Rule {
public:
	DatePartSimplificationRule(ExpressionRewriter &rewriter);

	unique_ptr<Expression> Apply(LogicalOperator &op, vector<Expression *> &bindings, bool &changes_made) override;
};

} // namespace graindb
