//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/storage/checkpoint/table_data_writer.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/storage/checkpoint_manager.hpp"
#include "graindb/common/unordered_map.hpp"

namespace graindb {
class UncompressedSegment;
class SegmentStatistics;

//! The table data writer is responsible for writing the data of a table to the block manager
class TableDataWriter {
public:
	TableDataWriter(CheckpointManager &manager, TableCatalogEntry &table);
	~TableDataWriter();

	void WriteTableData(Transaction &transaction);

private:
	void AppendData(Transaction &transaction, idx_t col_idx, Vector &data, idx_t count);

	void CreateSegment(idx_t col_idx);
	void FlushSegment(Transaction &transaction, idx_t col_idx);

	void WriteDataPointers();
	void VerifyDataPointers();

private:
	CheckpointManager &manager;
	TableCatalogEntry &table;

	vector<unique_ptr<UncompressedSegment>> segments;
	vector<unique_ptr<SegmentStatistics>> stats;

	vector<vector<DataPointer>> data_pointers;
};

} // namespace graindb
