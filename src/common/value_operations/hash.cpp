#include "graindb/common/types/hash.hpp"

#include "graindb/common/exception.hpp"
#include "graindb/common/value_operations/value_operations.hpp"

using namespace graindb;
using namespace std;

hash_t ValueOperations::Hash(const Value &op) {
	if (op.is_null) {
		return 0;
	}
	switch (op.type) {
	case TypeId::BOOL:
		return graindb::Hash(op.value_.boolean);
	case TypeId::INT8:
		return graindb::Hash(op.value_.tinyint);
	case TypeId::INT16:
		return graindb::Hash(op.value_.smallint);
	case TypeId::INT32:
		return graindb::Hash(op.value_.integer);
	case TypeId::INT64:
		return graindb::Hash(op.value_.bigint);
	case TypeId::FLOAT:
		return graindb::Hash(op.value_.float_);
	case TypeId::DOUBLE:
		return graindb::Hash(op.value_.double_);
	case TypeId::POINTER:
		return graindb::Hash(op.value_.pointer);
	case TypeId::VARCHAR:
		return graindb::Hash(op.str_value.c_str());
	default:
		throw NotImplementedException("Unimplemented type for hash");
	}
}
