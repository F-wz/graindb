#include "graindb/function/scalar/string_functions.hpp"
#include "graindb/common/types/string_type.hpp"

#include "graindb/common/exception.hpp"

using namespace std;

namespace graindb {

static bool prefix(const string_t &str, const string_t &pattern);

struct PrefixOperator {
	template <class TA, class TB, class TR> static inline TR Operation(TA left, TB right) {
		return prefix(left, right);
	}
};
static bool prefix(const string_t &str, const string_t &pattern) {
	auto str_size = str.GetSize();
	auto patt_length = pattern.GetSize();
	if (patt_length > str_size) {
		return false;
	}
	if (patt_length <= string_t::PREFIX_LENGTH) {
		// short prefix
		if (patt_length == 0) {
			// length = 0, return true
			return true;
		}

		// prefix early out
		const char *str_pref = str.GetPrefix();
		const char *patt_pref = pattern.GetPrefix();
		for (idx_t i = 0; i < patt_length; ++i) {
			if (str_pref[i] != patt_pref[i])
				return false;
		}
		return true;
	} else {
		// prefix early out
		const char *str_pref = str.GetPrefix();
		const char *patt_pref = pattern.GetPrefix();
		for (idx_t i = 0; i < string_t::PREFIX_LENGTH; ++i) {
			if (str_pref[i] != patt_pref[i]) {
				// early out
				return false;
			}
		}
		// compare the rest of the prefix
		const char *str_data = str.GetData();
		const char *patt_data = pattern.GetData();
		for (idx_t i = string_t::PREFIX_LENGTH; i < patt_length; ++i) {
			if (str_data[i] != patt_data[i]) {
				return false;
			}
		}
		return true;
	}
}

ScalarFunction PrefixFun::GetFunction() {
	return ScalarFunction("prefix",                             // name of the function
	                      {SQLType::VARCHAR, SQLType::VARCHAR}, // argument list
	                      SQLType::BOOLEAN,                     // return type
	                      ScalarFunction::BinaryFunction<string_t, string_t, bool, PrefixOperator, true>);
}

void PrefixFun::RegisterFunction(BuiltinFunctions &set) {
	set.AddFunction(GetFunction());
}

} // namespace graindb
