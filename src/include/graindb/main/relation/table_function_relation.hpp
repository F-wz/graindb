//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/main/relation/table_function_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/main/relation.hpp"

namespace graindb {

class TableFunctionRelation : public Relation {
public:
	TableFunctionRelation(ClientContext &context, string name, vector<Value> parameters);

	string name;
	vector<Value> parameters;
	vector<ColumnDefinition> columns;

public:
	unique_ptr<QueryNode> GetQueryNode() override;
	unique_ptr<TableRef> GetTableRef() override;

	const vector<ColumnDefinition> &Columns() override;
	string ToString(idx_t depth) override;
	string GetAlias() override;
};

} // namespace graindb
