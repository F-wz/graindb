//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/parsed_expression_iterator.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_expression.hpp"

#include <functional>

namespace graindb {

class ParsedExpressionIterator {
public:
	static void EnumerateChildren(const ParsedExpression &expression,
	                              std::function<void(const ParsedExpression &child)> callback);
};

} // namespace graindb
