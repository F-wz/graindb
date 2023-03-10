#include "graindb/catalog/catalog_entry/table_function_catalog_entry.hpp"

#include "graindb/common/exception.hpp"
#include "graindb/parser/constraints/list.hpp"
#include "graindb/parser/parsed_data/create_table_function_info.hpp"
#include "graindb/storage/storage_manager.hpp"

#include <algorithm>

using namespace graindb;
using namespace std;

TableFunctionCatalogEntry::TableFunctionCatalogEntry(Catalog *catalog, SchemaCatalogEntry *schema,
                                                     CreateTableFunctionInfo *info)
    : StandardEntry(CatalogType::TABLE_FUNCTION, schema, catalog, info->name), function(info->function) {
}
