//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/main/table_description.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/column_definition.hpp"

namespace graindb {

struct TableDescription {
	//! The schema of the table
	string schema;
	//! The table name of the table
	string table;
	//! The columns of the table
	vector<ColumnDefinition> columns;
};

} // namespace graindb
