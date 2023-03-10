//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/catalog/catalog_entry/index_catalog_entry.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/catalog/standard_entry.hpp"
#include "graindb/parser/parsed_data/create_index_info.hpp"

namespace graindb {

struct DataTableInfo;
class Index;

//! An index catalog entry
class IndexCatalogEntry : public StandardEntry {
public:
	//! Create a real TableCatalogEntry and initialize storage for it
	IndexCatalogEntry(Catalog *catalog, SchemaCatalogEntry *schema, CreateIndexInfo *info)
	    : StandardEntry(CatalogType::INDEX, schema, catalog, info->index_name), index(nullptr) {
	}
	~IndexCatalogEntry();

	Index *index;
	shared_ptr<DataTableInfo> info;
};

} // namespace graindb
