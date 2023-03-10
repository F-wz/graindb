//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/execution/operator/join/physical_join.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/execution/physical_operator.hpp"
#include "graindb/planner/operator/logical_comparison_join.hpp"

namespace graindb {

//! PhysicalJoin represents the base class of the join operators
class PhysicalJoin : public PhysicalOperator {
public:
	PhysicalJoin(LogicalOperator &op, PhysicalOperatorType type, JoinType join_type);

	JoinType type;

public:
	template <bool MATCH>
	static void ConstructSemiOrAntiJoinResult(DataChunk &left, DataChunk &result, bool found_match[]);
	static void ConstructMarkJoinResult(DataChunk &join_keys, DataChunk &left, DataChunk &result, bool found_match[],
	                                    bool has_null);
};

} // namespace graindb
