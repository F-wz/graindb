//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/function/scalar/sequence_functions.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/function/scalar_function.hpp"
#include "graindb/function/function_set.hpp"

namespace graindb {

struct NextvalFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

} // namespace graindb
