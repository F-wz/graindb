//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/function/function_set.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/function/aggregate_function.hpp"
#include "graindb/function/scalar_function.hpp"

namespace graindb {

template <class T> class FunctionSet {
public:
	FunctionSet(string name) : name(name) {
	}

	//! The name of the function set
	string name;
	//! The set of functions
	vector<T> functions;

public:
	void AddFunction(T function) {
		function.name = name;
		functions.push_back(function);
	}
};

class ScalarFunctionSet : public FunctionSet<ScalarFunction> {
public:
	ScalarFunctionSet(string name) : FunctionSet(move(name)) {
	}
};

class AggregateFunctionSet : public FunctionSet<AggregateFunction> {
public:
	AggregateFunctionSet(string name) : FunctionSet(move(name)) {
	}
};

} // namespace graindb
