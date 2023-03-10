//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/operator/logical_any_join.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_expression.hpp"
#include "graindb/planner/operator/logical_join.hpp"

namespace graindb {

//! LogicalAnyJoin represents a join with an arbitrary expression as JoinCondition
class LogicalAnyJoin : public LogicalJoin {
public:
	LogicalAnyJoin(JoinType type);

	//! The JoinCondition on which this join is performed
	unique_ptr<Expression> condition;

public:
	string ParamsToString() const override;
};

} // namespace graindb
