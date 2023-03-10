//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/operator/logical_order.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/planner/bound_query_node.hpp"
#include "graindb/planner/logical_operator.hpp"

namespace graindb {

//! LogicalOrder represents an ORDER BY clause, sorting the data
class LogicalOrder : public LogicalOperator {
public:
	LogicalOrder(vector<BoundOrderByNode> orders)
	    : LogicalOperator(LogicalOperatorType::ORDER_BY), orders(move(orders)) {
	}

	vector<BoundOrderByNode> orders;

public:
	vector<ColumnBinding> GetColumnBindings() override {
		return children[0]->GetColumnBindings();
	}

protected:
	void ResolveTypes() override {
		types = children[0]->types;
	}
};
} // namespace graindb
