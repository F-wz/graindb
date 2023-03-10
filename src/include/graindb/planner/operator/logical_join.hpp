//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/operator/logical_join.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/enums/join_type.hpp"
#include "graindb/common/unordered_set.hpp"
#include "graindb/planner/logical_operator.hpp"

namespace graindb {

//! LogicalJoin represents a join between two relations
class LogicalJoin : public LogicalOperator {
public:
	LogicalJoin(JoinType type, LogicalOperatorType logical_type = LogicalOperatorType::JOIN);

	// Gets the set of table references that are reachable from this node
	static void GetTableReferences(LogicalOperator &op, unordered_set<idx_t> &bindings);
	static void GetExpressionBindings(Expression &expr, unordered_set<idx_t> &bindings);

	//! The type of the join (INNER, OUTER, etc...)
	JoinType join_type;
	//! Table index used to refer to the MARK column (in case of a MARK join)
	idx_t mark_index;
	//! The columns of the LHS that are output by the join
	vector<idx_t> left_projection_map;
	//! The columns of the RHS that are output by the join
	vector<idx_t> right_projection_map;
	//! The columns of the merged scan (RAI) that are output by the join
	vector<idx_t> merge_projection_map;

public:
	vector<ColumnBinding> GetColumnBindings() override;
	ColumnBinding PushdownColumnBinding(ColumnBinding &binding) override;

protected:
	void ResolveTypes() override;
};

} // namespace graindb
