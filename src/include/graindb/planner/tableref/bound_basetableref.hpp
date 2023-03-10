//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/tableref/bound_basetableref.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/planner/bound_tableref.hpp"
#include "graindb/planner/logical_operator.hpp"

namespace graindb {
class TableCatalogEntry;

//! Represents a TableReference to a base table in the schema
class BoundBaseTableRef : public BoundTableRef {
public:
	BoundBaseTableRef(unique_ptr<LogicalOperator> get) : BoundTableRef(TableReferenceType::BASE_TABLE), get(move(get)) {
	}

	unique_ptr<LogicalOperator> get;
	idx_t table_index;
};
} // namespace graindb
