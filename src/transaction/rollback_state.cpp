#include "graindb/transaction/rollback_state.hpp"
#include "graindb/transaction/delete_info.hpp"
#include "graindb/transaction/update_info.hpp"

#include "graindb/storage/uncompressed_segment.hpp"
#include "graindb/storage/table/chunk_info.hpp"

#include "graindb/catalog/catalog_entry.hpp"
#include "graindb/catalog/catalog_set.hpp"

using namespace graindb;
using namespace std;

void RollbackState::RollbackEntry(UndoFlags type, data_ptr_t data) {
	switch (type) {
	case UndoFlags::CATALOG_ENTRY: {
		// undo this catalog entry
		CatalogEntry *catalog_entry = *((CatalogEntry **)data);
		assert(catalog_entry->set);
		catalog_entry->set->Undo(catalog_entry);
		break;
	}
	case UndoFlags::DELETE_TUPLE: {
		auto info = (DeleteInfo *)data;
		// reset the deleted flag on rollback
		info->vinfo->CommitDelete(NOT_DELETED_ID, info->rows, info->count);
		break;
	}
	case UndoFlags::UPDATE_TUPLE: {
		auto info = (UpdateInfo *)data;
		info->segment->RollbackUpdate(info);
		break;
	}
	default:
		assert(type == UndoFlags::EMPTY_ENTRY);
		break;
	}
}
