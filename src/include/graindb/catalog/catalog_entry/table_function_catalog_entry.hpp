//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/catalog/catalog_entry/table_function_catalog_entry.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/catalog/standard_entry.hpp"
#include "graindb/common/unordered_map.hpp"
#include "graindb/function/table_function.hpp"

namespace graindb {

class Catalog;
class Constraint;

struct CreateTableFunctionInfo;

//! A table function in the catalog
class TableFunctionCatalogEntry : public StandardEntry {
public:
	TableFunctionCatalogEntry(Catalog *catalog, SchemaCatalogEntry *schema, CreateTableFunctionInfo *info);

	//! The table function
	TableFunction function;
};
} // namespace graindb
