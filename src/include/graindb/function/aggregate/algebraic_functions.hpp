//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/function/aggregate/algebraic_functions.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/function/aggregate_function.hpp"

namespace graindb {

struct AvgFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct CovarSampFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct CovarPopFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct StdDevSampFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct StdDevPopFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct VarPopFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct VarSampFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

} // namespace graindb
