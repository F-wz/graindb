#include "graindb/parser/constraint.hpp"

#include "graindb/common/printer.hpp"
#include "graindb/common/serializer.hpp"
#include "graindb/parser/constraints/list.hpp"

using namespace graindb;
using namespace std;

void Constraint::Serialize(Serializer &serializer) {
	serializer.Write<ConstraintType>(type);
}

unique_ptr<Constraint> Constraint::Deserialize(Deserializer &source) {
	auto type = source.Read<ConstraintType>();
	switch (type) {
	case ConstraintType::NOT_NULL:
		return NotNullConstraint::Deserialize(source);
	case ConstraintType::CHECK:
		return CheckConstraint::Deserialize(source);
	case ConstraintType::UNIQUE:
		return UniqueConstraint::Deserialize(source);
	default:
		// don't know how to serialize this constraint type
		return nullptr;
	}
}

void Constraint::Print() {
	Printer::Print(ToString());
}
