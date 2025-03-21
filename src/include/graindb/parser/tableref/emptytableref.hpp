//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/tableref/emptytableref.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/tableref.hpp"

namespace graindb {
//! Represents a cross product
class EmptyTableRef : public TableRef {
public:
	EmptyTableRef() : TableRef(TableReferenceType::EMPTY) {
	}

public:
	bool Equals(const TableRef *other_) const override;

	unique_ptr<TableRef> Copy() override;

	//! Serializes a blob into a DummyTableRef
	void Serialize(Serializer &serializer) override;
	//! Deserializes a blob back into a DummyTableRef
	static unique_ptr<TableRef> Deserialize(Deserializer &source);
};
} // namespace graindb
