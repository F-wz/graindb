//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/execution/operator/schema/physical_create_edge.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/catalog/catalog_entry/table_catalog_entry.hpp"
#include "graindb/execution/physical_operator.hpp"
#include "graindb/parser/parsed_data/create_rai_info.hpp"
#include "graindb/storage/data_table.hpp"

namespace graindb {

class PhysicalCreateRAI : public PhysicalOperator {
public:
	PhysicalCreateRAI(LogicalOperator &op, string name, TableCatalogEntry &table, RAIDirection rai_direction,
	                  vector<column_t> column_ids, vector<TableCatalogEntry *> referenced_tables,
	                  vector<column_t> referenced_columns)
	    : PhysicalOperator(PhysicalOperatorType::CREATE_RAI, op.types), name(name), table(table),
	      rai_direction(rai_direction), column_ids(column_ids), referenced_tables(referenced_tables),
	      referenced_columns(referenced_columns) {
	}

	string name;
	TableCatalogEntry &table;
	RAIDirection rai_direction;
	vector<column_t> column_ids;
	vector<TableCatalogEntry *> referenced_tables;
	vector<column_t> referenced_columns;

public:
	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;
};
} // namespace graindb
