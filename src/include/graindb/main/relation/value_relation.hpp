//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/main/relation/value_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/main/relation.hpp"
#include "graindb/parser/parsed_expression.hpp"

namespace graindb {

class ValueRelation : public Relation {
public:
	ValueRelation(ClientContext &context, vector<vector<Value>> values, vector<string> names, string alias = "values");
	ValueRelation(ClientContext &context, string values, vector<string> names, string alias = "values");

	vector<vector<unique_ptr<ParsedExpression>>> expressions;
	vector<string> names;
	vector<ColumnDefinition> columns;
	string alias;

public:
	unique_ptr<QueryNode> GetQueryNode() override;

	const vector<ColumnDefinition> &Columns() override;
	string ToString(idx_t depth) override;
	string GetAlias() override;

	unique_ptr<TableRef> GetTableRef() override;
};

} // namespace graindb
