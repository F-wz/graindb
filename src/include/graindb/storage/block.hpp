//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/storage/block.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/common.hpp"
#include "graindb/storage/storage_info.hpp"
#include "graindb/common/file_buffer.hpp"

namespace graindb {

class Block : public FileBuffer {
public:
	Block(block_id_t id);

	block_id_t id;
};

} // namespace graindb
