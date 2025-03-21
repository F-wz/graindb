//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/tableref/subqueryref.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/query_node.hpp"
#include "graindb/parser/tableref.hpp"

namespace graindb {
//! Represents a subquery
class SubqueryRef : public TableRef {
public:
	SubqueryRef(unique_ptr<QueryNode> subquery, string alias = string());

	//! The subquery
	unique_ptr<QueryNode> subquery;
	//! Alises for the column names
	vector<string> column_name_alias;

public:
	bool Equals(const TableRef *other_) const override;

	unique_ptr<TableRef> Copy() override;

	//! Serializes a blob into a SubqueryRef
	void Serialize(Serializer &serializer) override;
	//! Deserializes a blob back into a SubqueryRef
	static unique_ptr<TableRef> Deserialize(Deserializer &source);
};
} // namespace graindb
