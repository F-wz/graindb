//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/main/relation/setop_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/main/relation.hpp"
#include "graindb/common/enums/set_operation_type.hpp"

namespace graindb {

class SetOpRelation : public Relation {
public:
	SetOpRelation(shared_ptr<Relation> left, shared_ptr<Relation> right, SetOperationType setop_type);

	shared_ptr<Relation> left;
	shared_ptr<Relation> right;
	SetOperationType setop_type;

public:
	unique_ptr<QueryNode> GetQueryNode() override;

	const vector<ColumnDefinition> &Columns() override;
	string ToString(idx_t depth) override;
	string GetAlias() override;
};

} // namespace graindb
