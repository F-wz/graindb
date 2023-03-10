//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/catalog/catalog_entry/scalar_function_catalog_entry.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/catalog/standard_entry.hpp"
#include "graindb/catalog/catalog_set.hpp"
#include "graindb/function/function.hpp"
#include "graindb/parser/parsed_data/create_scalar_function_info.hpp"

namespace graindb {

//! A table function in the catalog
class ScalarFunctionCatalogEntry : public StandardEntry {
public:
	ScalarFunctionCatalogEntry(Catalog *catalog, SchemaCatalogEntry *schema, CreateScalarFunctionInfo *info)
	    : StandardEntry(CatalogType::SCALAR_FUNCTION, schema, catalog, info->name), functions(info->functions) {
	}

	//! The scalar functions
	vector<ScalarFunction> functions;
};
} // namespace graindb
