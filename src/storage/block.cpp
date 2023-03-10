#include "graindb/storage/block.hpp"

using namespace graindb;
using namespace std;

Block::Block(block_id_t id) : FileBuffer(FileBufferType::BLOCK, Storage::BLOCK_ALLOC_SIZE), id(id) {
}
