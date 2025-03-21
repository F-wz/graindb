//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/main/relation/write_csv_relation.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/main/relation.hpp"

namespace graindb {

class WriteCSVRelation : public Relation {
public:
	WriteCSVRelation(shared_ptr<Relation> child, string csv_file);

	shared_ptr<Relation> child;
	string csv_file;
	vector<ColumnDefinition> columns;

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
