//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/parsed_data/create_table_function_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_data/create_info.hpp"
#include "graindb/function/table_function.hpp"

namespace graindb {

struct CreateTableFunctionInfo : public CreateInfo {
	CreateTableFunctionInfo(TableFunction function) : CreateInfo(CatalogType::TABLE_FUNCTION), function(function) {
		this->name = function.name;
	}

	//! Function name
	string name;
	//! The table function
	TableFunction function;
};

} // namespace graindb
