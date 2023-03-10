//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/tableref/bound_table_function.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/types/value.hpp"
#include "graindb/planner/expression.hpp"
#include "graindb/planner/bound_tableref.hpp"
#include "graindb/function/function.hpp"

namespace graindb {
class TableFunctionCatalogEntry;

//! Represents a reference to a table-producing function call
class BoundTableFunction : public BoundTableRef {
public:
	BoundTableFunction(TableFunctionCatalogEntry *function, idx_t bind_index)
	    : BoundTableRef(TableReferenceType::TABLE_FUNCTION), function(function), bind_index(bind_index) {
	}

	//! The function that is called
	TableFunctionCatalogEntry *function;
	//! The bind data of the function
	unique_ptr<FunctionData> bind_data;
	//! The set of parameters to use as input to the table-producing function
	vector<Value> parameters;
	//! The set of returned sql types
	vector<SQLType> return_types;
	//! The set of returned column names
	vector<string> names;
	//! The index in the bind context
	idx_t bind_index;
};
} // namespace graindb
