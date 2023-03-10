//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/execution/nested_loop_join.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/common.hpp"
#include "graindb/common/types/chunk_collection.hpp"
#include "graindb/common/types/vector.hpp"
#include "graindb/planner/operator/logical_comparison_join.hpp"

namespace graindb {

struct NestedLoopJoinInner {
	static idx_t Perform(idx_t &ltuple, idx_t &rtuple, DataChunk &left_conditions, DataChunk &right_conditions,
	                     SelectionVector &lvector, SelectionVector &rvector, vector<JoinCondition> &conditions);
};

struct NestedLoopJoinMark {
	static void Perform(DataChunk &left, ChunkCollection &right, bool found_match[], vector<JoinCondition> &conditions);
};

} // namespace graindb
