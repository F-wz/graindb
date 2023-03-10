//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/parsed_data/create_scalar_function_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_data/create_function_info.hpp"
#include "graindb/function/scalar_function.hpp"
#include "graindb/function/function_set.hpp"

namespace graindb {

struct CreateScalarFunctionInfo : public CreateFunctionInfo {
	CreateScalarFunctionInfo(ScalarFunction function) : CreateFunctionInfo(CatalogType::SCALAR_FUNCTION) {
		this->name = function.name;
		functions.push_back(function);
	}
	CreateScalarFunctionInfo(ScalarFunctionSet set)
	    : CreateFunctionInfo(CatalogType::SCALAR_FUNCTION), functions(move(set.functions)) {
		this->name = set.name;
	}

	vector<ScalarFunction> functions;
};

} // namespace graindb
