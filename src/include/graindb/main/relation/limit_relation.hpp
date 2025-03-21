//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/main/relation/limit_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/main/relation.hpp"

namespace graindb {

class LimitRelation : public Relation {
public:
	LimitRelation(shared_ptr<Relation> child, int64_t limit, int64_t offset);

	int64_t limit;
	int64_t offset;
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
