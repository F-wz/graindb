//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/constraints/check_constraint.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/string_util.hpp"
#include "graindb/parser/constraint.hpp"
#include "graindb/parser/parsed_expression.hpp"

namespace graindb {

//! The CheckConstraint contains an expression that must evaluate to TRUE for
//! every row in a table
class CheckConstraint : public Constraint {
public:
	CheckConstraint(unique_ptr<ParsedExpression> expression)
	    : Constraint(ConstraintType::CHECK), expression(move(expression)){};

	unique_ptr<ParsedExpression> expression;

public:
	string ToString() const override;

	unique_ptr<Constraint> Copy() override;

	//! Serialize to a stand-alone binary blob
	void Serialize(Serializer &serializer) override;
	//! Deserializes a CheckConstraint
	static unique_ptr<Constraint> Deserialize(Deserializer &source);
};

} // namespace graindb
