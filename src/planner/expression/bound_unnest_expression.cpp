#include "graindb/planner/expression/bound_unnest_expression.hpp"

#include "graindb/common/types/hash.hpp"
#include "graindb/common/string_util.hpp"

using namespace graindb;
using namespace std;

BoundUnnestExpression::BoundUnnestExpression(SQLType sql_return_type)
    : Expression(ExpressionType::BOUND_UNNEST, ExpressionClass::BOUND_UNNEST, GetInternalType(sql_return_type)),
      sql_return_type(sql_return_type) {
}

bool BoundUnnestExpression::IsFoldable() const {
	return false;
}

string BoundUnnestExpression::ToString() const {
	return "UNNEST(" + child->ToString() + ")";
}

hash_t BoundUnnestExpression::Hash() const {
	hash_t result = Expression::Hash();
	return CombineHash(result, graindb::Hash("unnest"));
}

bool BoundUnnestExpression::Equals(const BaseExpression *other_) const {
	if (!BaseExpression::Equals(other_)) {
		return false;
	}
	auto other = (BoundUnnestExpression *)other_;
	if (!Expression::Equals(child.get(), other->child.get())) {
		return false;
	}
	return true;
}

unique_ptr<Expression> BoundUnnestExpression::Copy() {
	auto copy = make_unique<BoundUnnestExpression>(sql_return_type);
	copy->child = child->Copy();
	return move(copy);
}
