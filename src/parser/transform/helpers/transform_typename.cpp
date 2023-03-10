#include "graindb/common/exception.hpp"
#include "graindb/common/string_util.hpp"
#include "graindb/parser/transformer.hpp"

using namespace graindb;
using namespace std;

SQLType Transformer::TransformTypeName(PGTypeName *type_name) {
	auto name = (reinterpret_cast<PGValue *>(type_name->names->tail->data.ptr_value)->val.str);
	// transform it to the SQL type
	return TransformStringToSQLType(name);
}
