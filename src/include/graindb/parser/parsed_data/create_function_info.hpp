//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/parsed_data/create_function_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_data/create_info.hpp"
#include "graindb/function/function.hpp"

namespace graindb {

struct CreateFunctionInfo : public CreateInfo {
	CreateFunctionInfo(CatalogType type) : CreateInfo(type) {
		assert(type == CatalogType::SCALAR_FUNCTION || type == CatalogType::AGGREGATE_FUNCTION);
	}

	//! Function name
	string name;
};

} // namespace graindb
