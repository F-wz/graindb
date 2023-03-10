//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/function/aggregate/nested_functions.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/function/aggregate_function.hpp"
#include "graindb/function/function_set.hpp"

namespace graindb {

struct ListBindData : public FunctionData {
	ListBindData() {
	}

	unique_ptr<FunctionData> Copy() override {
		return make_unique<ListBindData>();
	}
};

struct ListFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

} // namespace graindb
