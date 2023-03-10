//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/storage/table_statistics.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/common.hpp"
#include "graindb/common/types/data_chunk.hpp"

namespace graindb {

struct TableStatistics {
	idx_t estimated_cardinality;
};

} // namespace graindb
