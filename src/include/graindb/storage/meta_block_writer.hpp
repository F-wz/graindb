//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/storage/meta_block_writer.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/common.hpp"
#include "graindb/common/serializer.hpp"
#include "graindb/storage/block.hpp"
#include "graindb/storage/block_manager.hpp"

namespace graindb {

//! This struct is responsible for writing metadata to disk
class MetaBlockWriter : public Serializer {
public:
	MetaBlockWriter(BlockManager &manager);
	~MetaBlockWriter();

	BlockManager &manager;
	unique_ptr<Block> block;
	idx_t offset;

public:
	void Flush();

	void WriteData(const_data_ptr_t buffer, idx_t write_size) override;
};

} // namespace graindb
