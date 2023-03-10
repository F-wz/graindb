//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/transaction/rollback_state.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/transaction/undo_buffer.hpp"

namespace graindb {
class DataChunk;
class DataTable;
class WriteAheadLog;

class RollbackState {
public:
	RollbackState() {
	}

public:
	void RollbackEntry(UndoFlags type, data_ptr_t data);
};

} // namespace graindb
