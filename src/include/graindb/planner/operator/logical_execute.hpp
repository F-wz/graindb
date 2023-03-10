//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/operator/logical_execute.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/main/prepared_statement_data.hpp"
#include "graindb/planner/logical_operator.hpp"

namespace graindb {

class LogicalExecute : public LogicalOperator {
public:
	LogicalExecute(PreparedStatementData *prepared)
	    : LogicalOperator(LogicalOperatorType::EXECUTE), prepared(prepared) {
		assert(prepared);
		types = prepared->types;
	}

	PreparedStatementData *prepared;

protected:
	void ResolveTypes() override {
		// already resolved
	}
};
} // namespace graindb
