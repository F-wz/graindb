//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/tableref/table_function_ref.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_expression.hpp"
#include "graindb/parser/tableref.hpp"

namespace graindb {
//! Represents a Table producing function
class TableFunctionRef : public TableRef {
public:
	TableFunctionRef() : TableRef(TableReferenceType::TABLE_FUNCTION) {
	}

	unique_ptr<ParsedExpression> function;

public:
	string ToString() const override {
		return function->ToString();
	}

	bool Equals(const TableRef *other_) const override;

	unique_ptr<TableRef> Copy() override;

	//! Serializes a blob into a BaseTableRef
	void Serialize(Serializer &serializer) override;
	//! Deserializes a blob back into a BaseTableRef
	static unique_ptr<TableRef> Deserialize(Deserializer &source);
};
} // namespace graindb
