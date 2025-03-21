#include "graindb/storage/buffer/buffer_handle.hpp"
#include "graindb/storage/buffer_manager.hpp"

using namespace graindb;
using namespace std;

BufferHandle::BufferHandle(BufferManager &manager, block_id_t block_id, FileBuffer *node)
    : manager(manager), block_id(block_id), node(node) {
}

BufferHandle::~BufferHandle() {
	manager.Unpin(block_id);
}
