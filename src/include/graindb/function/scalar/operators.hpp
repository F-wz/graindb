//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/function/scalar/operators.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/function/scalar_function.hpp"
#include "graindb/function/function_set.hpp"

namespace graindb {

struct AddFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct SubtractFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct MultiplyFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct DivideFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct ModFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct LeftShiftFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct RightShiftFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct BitwiseAndFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct BitwiseOrFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct BitwiseXorFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

} // namespace graindb
