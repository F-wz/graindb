//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/function/table/sqlite_functions.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/function/table_function.hpp"

namespace graindb {

struct PragmaCollations {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct PragmaTableInfo {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct SQLiteMaster {
	static void RegisterFunction(BuiltinFunctions &set);
};

} // namespace graindb
