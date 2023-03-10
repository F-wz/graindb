#include "graindb/function/aggregate/algebraic_functions.hpp"
#include "graindb/function/aggregate_function.hpp"

using namespace std;

namespace graindb {

void BuiltinFunctions::RegisterAlgebraicAggregates() {
	Register<AvgFun>();

	Register<CovarSampFun>();
	Register<CovarPopFun>();

	Register<StdDevSampFun>();
	Register<StdDevPopFun>();
	Register<VarPopFun>();
	Register<VarSampFun>();
}

} // namespace graindb
