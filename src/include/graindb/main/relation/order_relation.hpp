//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/main/relation/order_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/main/relation.hpp"
#include "graindb/parser/parsed_expression.hpp"
#include "graindb/parser/result_modifier.hpp"

namespace graindb {

class OrderRelation : public Relation {
public:
	OrderRelation(shared_ptr<Relation> child, vector<OrderByNode> orders);

	vector<OrderByNode> orders;
	shared_ptr<Relation> child;

public:
	unique_ptr<QueryNode> GetQueryNode() override;

	const vector<ColumnDefinition> &Columns() override;
	string ToString(idx_t depth) override;
	string GetAlias() override;

public:
	bool InheritsColumnBindings() override {
		return true;
	}
	Relation *ChildRelation() override {
		return child.get();
	}
};

} // namespace graindb
