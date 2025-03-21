//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/operator/logical_get.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/catalog/catalog_entry/table_catalog_entry.hpp"
#include "graindb/planner/logical_operator.hpp"
#include "graindb/storage/data_table.hpp"
#include "graindb/storage/rai.hpp"

namespace graindb {

//! LogicalGet represents a scan operation from a data source
class LogicalGet : public LogicalOperator {
public:
	LogicalGet(idx_t table_index);
	LogicalGet(TableCatalogEntry *table, idx_t table_index);
	LogicalGet(TableCatalogEntry *table, idx_t table_index, vector<column_t> column_ids);

	idx_t EstimateCardinality() override;

	//! The base table to retrieve data from
	TableCatalogEntry *table;
	//! The table index in the current bind context
	idx_t table_index;
	//! The table alias in the current bind context
	string table_alias;
	//! Bound column IDs
	vector<column_t> column_ids;
	//! Filters pushed down for table scan
	vector<TableFilter> tableFilters;
	string ParamsToString() const override;

public:
	vector<ColumnBinding> GetColumnBindings() override;
	ColumnBinding PushdownColumnBinding(ColumnBinding &binding) override;

protected:
	void ResolveTypes() override;
};
} // namespace graindb
