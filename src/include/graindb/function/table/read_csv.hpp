//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/function/table/read_csv.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/function/table_function.hpp"

namespace graindb {

struct ReadCSVTableFunction {
	static void RegisterFunction(BuiltinFunctions &set);
};

} // namespace graindb
