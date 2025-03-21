//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/function/scalar/nested_functions.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/function/scalar_function.hpp"
#include "graindb/function/function_set.hpp"

namespace graindb {

struct VariableReturnBindData : public FunctionData {
	SQLType stype;

	VariableReturnBindData(SQLType stype) : stype(stype) {
	}

	unique_ptr<FunctionData> Copy() override {
		return make_unique<VariableReturnBindData>(stype);
	}
};

struct StructPackFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct ListValueFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct StructExtractBindData : public FunctionData {
	string key;
	idx_t index;
	TypeId type;

	StructExtractBindData(string key, idx_t index, TypeId type) : key(key), index(index), type(type) {
	}

	unique_ptr<FunctionData> Copy() override {
		return make_unique<StructExtractBindData>(key, index, type);
	}
};

struct StructExtractFun {
	static void RegisterFunction(BuiltinFunctions &set);
};

} // namespace graindb
