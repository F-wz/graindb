#include "graindb/function/aggregate/nested_functions.hpp"

using namespace std;

namespace graindb {

void BuiltinFunctions::RegisterNestedAggregates() {
	Register<ListFun>();
}

} // namespace graindb
