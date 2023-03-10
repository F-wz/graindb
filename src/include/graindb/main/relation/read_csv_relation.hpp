//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/main/relation/read_csv_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/main/relation.hpp"

namespace graindb {

class ReadCSVRelation : public Relation {
public:
	ReadCSVRelation(ClientContext &context, string csv_file, vector<ColumnDefinition> columns, string alias = string());

	string csv_file;
	string alias;
	vector<ColumnDefinition> columns;

public:
	unique_ptr<QueryNode> GetQueryNode() override;
	const vector<ColumnDefinition> &Columns() override;
	string ToString(idx_t depth) override;
	string GetAlias() override;
	unique_ptr<TableRef> GetTableRef() override;
};

} // namespace graindb
