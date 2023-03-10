//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/bound_query_node.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/planner/expression.hpp"
#include "graindb/planner/bound_result_modifier.hpp"
#include "graindb/parser/query_node.hpp"

namespace graindb {

//! Bound equivalent of QueryNode
class BoundQueryNode {
public:
	BoundQueryNode(QueryNodeType type) : type(type) {
	}
	virtual ~BoundQueryNode() {
	}

	//! The type of the query node, either SetOperation or Select
	QueryNodeType type;
	//! The result modifiers that should be applied to this query node
	vector<unique_ptr<BoundResultModifier>> modifiers;

	//! The names returned by this QueryNode.
	vector<string> names;
	//! The types returned by this QueryNode.
	vector<SQLType> types;

public:
	virtual idx_t GetRootIndex() = 0;
};

}; // namespace graindb
