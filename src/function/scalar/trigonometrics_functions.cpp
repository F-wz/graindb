#include "graindb/function/scalar/trigonometric_functions.hpp"
#include "graindb/common/vector_operations/vector_operations.hpp"
#include "graindb/common/exception.hpp"

using namespace std;

namespace graindb {

void BuiltinFunctions::RegisterTrigonometricsFunctions() {
	Register<SinFun>();
	Register<CosFun>();
	Register<TanFun>();
	Register<AsinFun>();
	Register<AcosFun>();
	Register<AtanFun>();
	Register<CotFun>();
	Register<Atan2Fun>();
}

} // namespace graindb
