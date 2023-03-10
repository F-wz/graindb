//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/main/relation/table_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/main/relation.hpp"
#include "graindb/main/table_description.hpp"

namespace graindb {

class TableRelation : public Relation {
public:
	TableRelation(ClientContext &context, unique_ptr<TableDescription> description);

	unique_ptr<TableDescription> description;

public:
	unique_ptr<QueryNode> GetQueryNode() override;

	const vector<ColumnDefinition> &Columns() override;
	string ToString(idx_t depth) override;
	string GetAlias() override;

	unique_ptr<TableRef> GetTableRef() override;

	void Update(string update, string condition = string()) override;
	void Delete(string condition = string()) override;
};

} // namespace graindb
