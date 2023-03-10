#include "graindb/planner/expression/bound_reference_expression.hpp"

#include "graindb/common/serializer.hpp"
#include "graindb/common/types/hash.hpp"

using namespace graindb;
using namespace std;

BoundReferenceExpression::BoundReferenceExpression(string alias, TypeId type, idx_t index)
    : Expression(ExpressionType::BOUND_REF, ExpressionClass::BOUND_REF, type), index(index) {
	this->alias = alias;
}
BoundReferenceExpression::BoundReferenceExpression(TypeId type, idx_t index)
    : BoundReferenceExpression(string(), type, index) {
}

string BoundReferenceExpression::ToString() const {
	return "#" + std::to_string(index);
}

bool BoundReferenceExpression::Equals(const BaseExpression *other_) const {
	if (!BaseExpression::Equals(other_)) {
		return false;
	}
	auto other = (BoundReferenceExpression *)other_;
	return other->index == index;
}

hash_t BoundReferenceExpression::Hash() const {
	return CombineHash(Expression::Hash(), graindb::Hash<idx_t>(index));
}

unique_ptr<Expression> BoundReferenceExpression::Copy() {
	return make_unique<BoundReferenceExpression>(alias, return_type, index);
}
