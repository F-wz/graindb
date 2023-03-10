//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/tableref/bound_expressionlistref.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/planner/bound_tableref.hpp"
#include "graindb/planner/expression.hpp"

namespace graindb {
//! Represents a TableReference to a base table in the schema
class BoundExpressionListRef : public BoundTableRef {
public:
	BoundExpressionListRef() : BoundTableRef(TableReferenceType::EXPRESSION_LIST) {
	}

	//! The bound VALUES list
	vector<vector<unique_ptr<Expression>>> values;
	//! The generated names of the values list
	vector<string> names;
	//! The types of the values list
	vector<SQLType> types;
	//! The index in the bind context
	idx_t bind_index;
};
} // namespace graindb
