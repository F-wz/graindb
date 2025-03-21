//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/operator/logical_projection.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/planner/logical_operator.hpp"

namespace graindb {

//! LogicalProjection represents the projection list in a SELECT clause
class LogicalProjection : public LogicalOperator {
public:
	LogicalProjection(idx_t table_index, vector<unique_ptr<Expression>> select_list);

	idx_t table_index;

public:
	vector<ColumnBinding> GetColumnBindings() override;
	ColumnBinding PushdownColumnBinding(ColumnBinding &binding) override;

protected:
	void ResolveTypes() override;
};
} // namespace graindb
