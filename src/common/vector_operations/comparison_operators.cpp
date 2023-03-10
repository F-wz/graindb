//===--------------------------------------------------------------------===//
// comparison_operators.cpp
// Description: This file contains the implementation of the comparison
// operations == != >= <= > <
//===--------------------------------------------------------------------===//

#include "graindb/common/operator/comparison_operators.hpp"

#include "graindb/common/vector_operations/binary_executor.hpp"
#include "graindb/common/vector_operations/vector_operations.hpp"

using namespace graindb;
using namespace std;

struct ComparisonExecutor {
private:
	template <class T, class OP, bool IGNORE_NULL = false>
	static inline void TemplatedExecute(Vector &left, Vector &right, Vector &result, idx_t count) {
		BinaryExecutor::Execute<T, T, bool, OP, IGNORE_NULL>(left, right, result, count);
	}

public:
	template <class OP> static inline void Execute(Vector &left, Vector &right, Vector &result, idx_t count) {
		assert(left.type == right.type && result.type == TypeId::BOOL);
		// the inplace loops take the result as the last parameter
		switch (left.type) {
		case TypeId::BOOL:
		case TypeId::INT8:
			TemplatedExecute<int8_t, OP>(left, right, result, count);
			break;
		case TypeId::INT16:
			TemplatedExecute<int16_t, OP>(left, right, result, count);
			break;
		case TypeId::INT32:
			TemplatedExecute<int32_t, OP>(left, right, result, count);
			break;
		case TypeId::INT64:
			TemplatedExecute<int64_t, OP>(left, right, result, count);
			break;
		case TypeId::POINTER:
			TemplatedExecute<uintptr_t, OP>(left, right, result, count);
			break;
		case TypeId::FLOAT:
			TemplatedExecute<float, OP>(left, right, result, count);
			break;
		case TypeId::DOUBLE:
			TemplatedExecute<double, OP>(left, right, result, count);
			break;
		case TypeId::VARCHAR:
			TemplatedExecute<string_t, OP, true>(left, right, result, count);
			break;
		default:
			throw InvalidTypeException(left.type, "Invalid type for comparison");
		}
	}
};

void VectorOperations::Equals(Vector &left, Vector &right, Vector &result, idx_t count) {
	ComparisonExecutor::Execute<graindb::Equals>(left, right, result, count);
}

void VectorOperations::NotEquals(Vector &left, Vector &right, Vector &result, idx_t count) {
	ComparisonExecutor::Execute<graindb::NotEquals>(left, right, result, count);
}

void VectorOperations::GreaterThanEquals(Vector &left, Vector &right, Vector &result, idx_t count) {
	ComparisonExecutor::Execute<graindb::GreaterThanEquals>(left, right, result, count);
}

void VectorOperations::LessThanEquals(Vector &left, Vector &right, Vector &result, idx_t count) {
	ComparisonExecutor::Execute<graindb::LessThanEquals>(left, right, result, count);
}

void VectorOperations::GreaterThan(Vector &left, Vector &right, Vector &result, idx_t count) {
	ComparisonExecutor::Execute<graindb::GreaterThan>(left, right, result, count);
}

void VectorOperations::LessThan(Vector &left, Vector &right, Vector &result, idx_t count) {
	ComparisonExecutor::Execute<graindb::LessThan>(left, right, result, count);
}
