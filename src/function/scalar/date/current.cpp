#include "graindb/function/scalar/date_functions.hpp"

#include "graindb/common/exception.hpp"
#include "graindb/common/types/date.hpp"
#include "graindb/common/types/timestamp.hpp"
#include "graindb/common/vector_operations/vector_operations.hpp"

using namespace std;

namespace graindb {

static void current_time_function(DataChunk &input, ExpressionState &state, Vector &result) {
	assert(input.column_count() == 0);

	auto val = Value::INTEGER(Timestamp::GetTime(Timestamp::GetCurrentTimestamp()));
	result.Reference(val);
}

static void current_date_function(DataChunk &input, ExpressionState &state, Vector &result) {
	assert(input.column_count() == 0);

	auto val = Value::INTEGER(Timestamp::GetDate(Timestamp::GetCurrentTimestamp()));
	result.Reference(val);
}

static void current_timestamp_function(DataChunk &input, ExpressionState &state, Vector &result) {
	assert(input.column_count() == 0);

	auto val = Value::TIMESTAMP(Timestamp::GetCurrentTimestamp());
	result.Reference(val);
}

void CurrentTimeFun::RegisterFunction(BuiltinFunctions &set) {
	set.AddFunction(ScalarFunction("current_time", {}, SQLType::TIME, current_time_function));
}

void CurrentDateFun::RegisterFunction(BuiltinFunctions &set) {
	set.AddFunction(ScalarFunction("current_date", {}, SQLType::DATE, current_date_function));
}

void CurrentTimestampFun::RegisterFunction(BuiltinFunctions &set) {
	set.AddFunction({"now", "current_timestamp"}, ScalarFunction({}, SQLType::TIMESTAMP, current_timestamp_function));
}

} // namespace graindb
