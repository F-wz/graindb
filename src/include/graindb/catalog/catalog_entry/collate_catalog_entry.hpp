//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/catalog/catalog_entry/collate_catalog_entry.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/catalog/standard_entry.hpp"
#include "graindb/function/function.hpp"
#include "graindb/parser/parsed_data/create_collation_info.hpp"

namespace graindb {

//! A collation catalog entry
class CollateCatalogEntry : public StandardEntry {
public:
	CollateCatalogEntry(Catalog *catalog, SchemaCatalogEntry *schema, CreateCollationInfo *info)
	    : StandardEntry(CatalogType::COLLATION, schema, catalog, info->name), function(info->function),
	      combinable(info->combinable), not_required_for_equality(info->not_required_for_equality) {
	}

	//! The collation function to push in case collation is required
	ScalarFunction function;
	//! Whether or not the collation can be combined with other collations.
	bool combinable;
	//! Whether or not the collation is required for equality comparisons or not. For many collations a binary comparison for equality comparisons is correct, allowing us to skip the collation in these cases which greatly speeds up processing.
	bool not_required_for_equality;
};
} // namespace graindb
