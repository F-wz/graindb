#include "graindb/parser/constraints/not_null_constraint.hpp"

#include "graindb/common/serializer.hpp"

using namespace std;
using namespace graindb;

string NotNullConstraint::ToString() const {
	return "NOT NULL Constraint";
}

unique_ptr<Constraint> NotNullConstraint::Copy() {
	return make_unique<NotNullConstraint>(index);
}

void NotNullConstraint::Serialize(Serializer &serializer) {
	Constraint::Serialize(serializer);
	serializer.Write<idx_t>(index);
}

unique_ptr<Constraint> NotNullConstraint::Deserialize(Deserializer &source) {
	auto index = source.Read<idx_t>();
	return make_unique_base<Constraint, NotNullConstraint>(index);
}
