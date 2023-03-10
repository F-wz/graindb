#include "graindb/function/scalar/nested_functions.hpp"

using namespace std;

namespace graindb {

void BuiltinFunctions::RegisterNestedFunctions() {
	Register<StructPackFun>();
	Register<StructExtractFun>();
	Register<ListValueFun>();
}

} // namespace graindb
