//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/expression_map.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/unordered_map.hpp"
#include "graindb/common/unordered_set.hpp"
#include "graindb/parser/base_expression.hpp"

namespace graindb {
class Expression;

struct ExpressionHashFunction {
	uint64_t operator()(const BaseExpression *const &expr) const {
		return (uint64_t)expr->Hash();
	}
};

struct ExpressionEquality {
	bool operator()(const BaseExpression *const &a, const BaseExpression *const &b) const {
		return a->Equals(b);
	}
};

template <typename T>
using expression_map_t = unordered_map<BaseExpression *, T, ExpressionHashFunction, ExpressionEquality>;

using expression_set_t = unordered_set<BaseExpression *, ExpressionHashFunction, ExpressionEquality>;

} // namespace graindb
