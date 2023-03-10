//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/operator/logical_prepare.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/unordered_map.hpp"
#include "graindb/common/unordered_set.hpp"
#include "graindb/main/prepared_statement_data.hpp"
#include "graindb/planner/logical_operator.hpp"

namespace graindb {

class TableCatalogEntry;

class LogicalPrepare : public LogicalOperator {
public:
	LogicalPrepare(string name, unique_ptr<PreparedStatementData> prepared, unique_ptr<LogicalOperator> logical_plan)
	    : LogicalOperator(LogicalOperatorType::PREPARE), name(name), prepared(move(prepared)) {
		children.push_back(move(logical_plan));
	}

	string name;
	unique_ptr<PreparedStatementData> prepared;

protected:
	void ResolveTypes() override {
		types.push_back(TypeId::BOOL);
	}
};
} // namespace graindb
