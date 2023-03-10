//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/tableref/joinref.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/enums/join_type.hpp"
#include "graindb/common/unordered_set.hpp"
#include "graindb/parser/parsed_expression.hpp"
#include "graindb/parser/tableref.hpp"

namespace graindb {
//! Represents a JOIN between two expressions
class JoinRef : public TableRef {
public:
	JoinRef() : TableRef(TableReferenceType::JOIN) {
	}

	//! The left hand side of the join
	unique_ptr<TableRef> left;
	//! The right hand side of the join
	unique_ptr<TableRef> right;
	//! The join condition
	unique_ptr<ParsedExpression> condition;
	//! The join type
	JoinType type;
	//! The set of USING columns (if any)
	vector<string> using_columns;
	//! The alias
	string alias;

public:
	bool Equals(const TableRef *other_) const override;

	unique_ptr<TableRef> Copy() override;

	//! Serializes a blob into a JoinRef
	void Serialize(Serializer &serializer) override;
	//! Deserializes a blob back into a JoinRef
	static unique_ptr<TableRef> Deserialize(Deserializer &source);
};
} // namespace graindb
