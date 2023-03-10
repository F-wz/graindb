#include "graindb/function/aggregate/distributive_functions.hpp"
#include "graindb/common/exception.hpp"
#include "graindb/common/types/null_value.hpp"
#include "graindb/common/vector_operations/vector_operations.hpp"
#include "graindb/function/aggregate_function.hpp"

using namespace std;

namespace graindb {

void BuiltinFunctions::RegisterDistributiveAggregates() {
	Register<BitAndFun>();
	Register<BitOrFun>();
	Register<BitXorFun>();
	Register<CountStarFun>();
	Register<CountFun>();
	Register<FirstFun>();
	Register<MaxFun>();
	Register<MinFun>();
	Register<SumFun>();
	Register<StringAggFun>();
}

} // namespace graindb
