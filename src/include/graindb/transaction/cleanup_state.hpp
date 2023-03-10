//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/transaction/cleanup_state.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/transaction/undo_buffer.hpp"
#include "graindb/common/types/data_chunk.hpp"

namespace graindb {
class DataTable;

struct DeleteInfo;
struct UpdateInfo;

class CleanupState {
public:
	CleanupState();
	~CleanupState();

public:
	void CleanupEntry(UndoFlags type, data_ptr_t data);

private:
	// data for index cleanup
	DataTable *current_table;
	DataChunk chunk;
	row_t row_numbers[STANDARD_VECTOR_SIZE];
	idx_t count;

private:
	void CleanupDelete(DeleteInfo *info);
	void CleanupUpdate(UpdateInfo *info);

	void Flush();
};

} // namespace graindb
