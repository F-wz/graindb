#include "graindb/parser/base_expression.hpp"

#include "graindb/common/printer.hpp"

using namespace graindb;
using namespace std;

void BaseExpression::Print() {
	Printer::Print(ToString());
}

bool BaseExpression::Equals(const BaseExpression *other) const {
	if (!other) {
		return false;
	}
	if (this->expression_class != other->expression_class || this->type != other->type) {
		return false;
	}
	return true;
}
