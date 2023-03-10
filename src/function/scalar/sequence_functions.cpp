#include "graindb/function/scalar/sequence_functions.hpp"

using namespace std;

namespace graindb {

void BuiltinFunctions::RegisterSequenceFunctions() {
	Register<NextvalFun>();
}

} // namespace graindb
