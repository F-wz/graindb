//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/bound_result_modifier.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/result_modifier.hpp"
#include "graindb/planner/expression.hpp"

namespace graindb {

//! A ResultModifier
class BoundResultModifier {
public:
	BoundResultModifier(ResultModifierType type) : type(type) {
	}
	virtual ~BoundResultModifier() {
	}

	ResultModifierType type;
};

struct BoundOrderByNode {
	BoundOrderByNode() {
	}
	BoundOrderByNode(OrderType type, unique_ptr<Expression> expression) : type(type), expression(move(expression)) {
	}

	OrderType type;
	unique_ptr<Expression> expression;
};

class BoundLimitModifier : public BoundResultModifier {
public:
	BoundLimitModifier() : BoundResultModifier(ResultModifierType::LIMIT_MODIFIER) {
	}

	//! LIMIT count
	int64_t limit = -1;
	//! OFFSET
	int64_t offset = -1;
};

class BoundOrderModifier : public BoundResultModifier {
public:
	BoundOrderModifier() : BoundResultModifier(ResultModifierType::ORDER_MODIFIER) {
	}

	//! List of order nodes
	vector<BoundOrderByNode> orders;
};

class BoundDistinctModifier : public BoundResultModifier {
public:
	BoundDistinctModifier() : BoundResultModifier(ResultModifierType::DISTINCT_MODIFIER) {
	}

	//! list of distinct on targets (if any)
	vector<unique_ptr<Expression>> target_distincts;
};

} // namespace graindb
