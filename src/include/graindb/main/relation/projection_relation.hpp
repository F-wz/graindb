//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/main/relation/projection_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/main/relation.hpp"
#include "graindb/parser/parsed_expression.hpp"

namespace graindb {

class ProjectionRelation : public Relation {
public:
	ProjectionRelation(shared_ptr<Relation> child, vector<unique_ptr<ParsedExpression>> expressions,
	                   vector<string> aliases);

	vector<unique_ptr<ParsedExpression>> expressions;
	vector<ColumnDefinition> columns;
	shared_ptr<Relation> child;

public:
	unique_ptr<QueryNode> GetQueryNode() override;

	const vector<ColumnDefinition> &Columns() override;
	string ToString(idx_t depth) override;
	string GetAlias() override;
};

} // namespace graindb
