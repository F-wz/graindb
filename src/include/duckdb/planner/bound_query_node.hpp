//===----------------------------------------------------------------------===//
//                         GrainDB
//
// duckdb/planner/bound_query_node.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/planner/expression.hpp"
#include "duckdb/planner/bound_result_modifier.hpp"
#include "duckdb/parser/query_node.hpp"

namespace duckdb {

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

}; // namespace duckdb
