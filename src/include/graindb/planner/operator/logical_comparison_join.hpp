//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/operator/logical_comparison_join.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/constants.hpp"
#include "graindb/common/unordered_set.hpp"
#include "graindb/optimizer/join_order/query_graph.hpp"
#include "graindb/planner/joinside.hpp"
#include "graindb/planner/operator/logical_join.hpp"
#include "graindb/storage/rai.hpp"

namespace graindb {

//! LogicalComparisonJoin represents a join that involves comparisons between the LHS and RHS
class LogicalComparisonJoin : public LogicalJoin {
public:
	LogicalComparisonJoin(JoinType type, LogicalOperatorType logical_type = LogicalOperatorType::COMPARISON_JOIN);

	//! The conditions of the join
	vector<JoinCondition> conditions;
	bool enable_lookup_join;

public:
	string ParamsToString() const override;

public:
	static unique_ptr<LogicalOperator> CreateJoin(JoinType type, unique_ptr<LogicalOperator> left_child,
	                                              unique_ptr<LogicalOperator> right_child,
	                                              unordered_set<idx_t> &left_bindings,
	                                              unordered_set<idx_t> &right_bindings,
	                                              vector<unique_ptr<Expression>> &expressions);
};

} // namespace graindb
