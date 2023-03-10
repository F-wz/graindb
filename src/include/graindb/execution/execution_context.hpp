//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/execution/execution_context.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/types/data_chunk.hpp"
#include "graindb/execution/physical_operator.hpp"
#include "graindb/main/query_result.hpp"

namespace graindb {
class GrainDB;

class ExecutionContext {
public:
	unique_ptr<PhysicalOperator> physical_plan;
	unique_ptr<PhysicalOperatorState> physical_state;

public:
	void Reset() {
		physical_plan = nullptr;
		physical_state = nullptr;
	}
};
} // namespace graindb
