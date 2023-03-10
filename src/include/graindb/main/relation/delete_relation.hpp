//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/main/relation/delete_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/main/relation.hpp"
#include "graindb/parser/parsed_expression.hpp"

namespace graindb {

class DeleteRelation : public Relation {
public:
	DeleteRelation(ClientContext &context, unique_ptr<ParsedExpression> condition, string schema_name,
	               string table_name);

	vector<ColumnDefinition> columns;
	unique_ptr<ParsedExpression> condition;
	string schema_name;
	string table_name;

public:
	unique_ptr<QueryNode> GetQueryNode() override;
	BoundStatement Bind(Binder &binder) override;
	const vector<ColumnDefinition> &Columns() override;
	string ToString(idx_t depth) override;
	bool IsReadOnly() override {
		return false;
	}
};

} // namespace graindb
