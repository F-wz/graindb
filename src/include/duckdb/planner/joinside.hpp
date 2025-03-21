//===----------------------------------------------------------------------===//
//                         GrainDB
//
// duckdb/planner/joinside.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/unordered_set.hpp"
#include "duckdb/planner/expression.hpp"
#include "duckdb/storage/rai.hpp"

namespace duckdb {

//! JoinCondition represents a left-right comparison join condition
struct JoinCondition {
public:
	JoinCondition() : null_values_are_equal(false) {
	}

	//! Turns the JoinCondition into an expression; note that this destroys the JoinCondition as the expression inherits
	//! the left/right expressions
	static unique_ptr<Expression> CreateExpression(JoinCondition cond);

public:
	unique_ptr<Expression> left;
	unique_ptr<Expression> right;
	ExpressionType comparison;
	//! NULL values are equal for just THIS JoinCondition (instead of the entire join).
	//! This is only supported by the HashJoin and can only be used in equality comparisons.
	bool null_values_are_equal = false;
	//! RAI info binded to this join condition
	vector<unique_ptr<RAIInfo>> rais;

	string ToString() {
		string result = ExpressionTypeToString(comparison) + "(" + left->GetName() + ", " + right->GetName() + ")";
		if (rais.size() != 0 ) {
			result += "[";
			for (auto &rai: rais) {
				result += rai->ToString();
				result += "\n";
			}
			result += "]";
		}
		return result;
	}
};

// enum class JoinSide : uint8_t {
// 	NONE, LEFT, RIGHT, BOTH
// };

class JoinSide {
public:
	enum join_value : uint8_t { NONE, LEFT, RIGHT, BOTH };

	JoinSide() = default;
	constexpr JoinSide(join_value val) : value(val) {
	}

	bool operator==(JoinSide a) const {
		return value == a.value;
	}
	bool operator!=(JoinSide a) const {
		return value != a.value;
	}

	static JoinSide CombineJoinSide(JoinSide left, JoinSide right);
	static JoinSide GetJoinSide(idx_t table_binding, unordered_set<idx_t> &left_bindings,
	                            unordered_set<uint64_t> &right_bindings);
	static JoinSide GetJoinSide(Expression &expression, unordered_set<idx_t> &left_bindings,
	                            unordered_set<idx_t> &right_bindings);
	static JoinSide GetJoinSide(unordered_set<idx_t> bindings, unordered_set<idx_t> &left_bindings,
	                            unordered_set<idx_t> &right_bindings);

private:
	join_value value;
};

}; // namespace duckdb
