//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/storage/meta_block_reader.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/common.hpp"
#include "graindb/common/serializer.hpp"
#include "graindb/storage/block.hpp"
#include "graindb/storage/buffer_manager.hpp"

namespace graindb {
//! This struct is responsible for reading meta data from disk
class MetaBlockReader : public Deserializer {
public:
	MetaBlockReader(BufferManager &manager, block_id_t block);

	BufferManager &manager;
	unique_ptr<BufferHandle> handle;
	idx_t offset;
	block_id_t next_block;

public:
	//! Read content of size read_size into the buffer
	void ReadData(data_ptr_t buffer, idx_t read_size) override;

private:
	void ReadNewBlock(block_id_t id);
};
} // namespace graindb
