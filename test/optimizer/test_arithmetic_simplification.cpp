#include "catch.hpp"
#include "graindb/common/helper.hpp"
#include "expression_helper.hpp"
#include "graindb/optimizer/rule/arithmetic_simplification.hpp"

using namespace graindb;
using namespace std;

TEST_CASE("Arithmetic simplification test", "[optimizer]") {
	ExpressionHelper helper;

	REQUIRE(helper.AddColumns("X INTEGER").empty());

	helper.AddRule<ArithmeticSimplificationRule>();

	string input, expected_output;

	input = "X+0";
	expected_output = "X";
	REQUIRE(helper.VerifyRewrite(input, expected_output));

	input = "0+X";
	expected_output = "X";
	REQUIRE(helper.VerifyRewrite(input, expected_output));

	input = "X-0";
	expected_output = "X";
	REQUIRE(helper.VerifyRewrite(input, expected_output));

	input = "X*1";
	expected_output = "X";
	REQUIRE(helper.VerifyRewrite(input, expected_output));

	input = "X/0";
	expected_output = "NULL";
	REQUIRE(helper.VerifyRewrite(input, expected_output));

	input = "X/1";
	expected_output = "X";
	REQUIRE(helper.VerifyRewrite(input, expected_output));
}
