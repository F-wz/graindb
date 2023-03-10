#include "graindb/catalog/catalog_entry/index_catalog_entry.hpp"
#include "graindb/storage/data_table.hpp"

namespace graindb {

IndexCatalogEntry::~IndexCatalogEntry() {
	// remove the associated index from the info
	if (!info || !index) {
		return;
	}
	for (idx_t i = 0; i < info->indexes.size(); i++) {
		if (info->indexes[i].get() == index) {
			info->indexes.erase(info->indexes.begin() + i);
			break;
		}
	}
}

} // namespace graindb
