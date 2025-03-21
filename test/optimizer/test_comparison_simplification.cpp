#include "catch.hpp"
#include "graindb/common/helper.hpp"
#include "expression_helper.hpp"
#include "graindb/optimizer/rule/comparison_simplification.hpp"

using namespace graindb;
using namespace std;

TEST_CASE("Comparison simplification test", "[optimizer]") {
	ExpressionHelper helper;

	REQUIRE(helper.AddColumns("X INTEGER").empty());

	helper.AddRule<ComparisonSimplificationRule>();

	string input, expected_output;

	input = "X=NULL";
	expected_output = "NULL";
	REQUIRE(helper.VerifyRewrite(input, expected_output));

	input = "X>NULL";
	expected_output = "NULL";
	REQUIRE(helper.VerifyRewrite(input, expected_output));

	input = "NULL>X";
	expected_output = "NULL";
	REQUIRE(helper.VerifyRewrite(input, expected_output));
}
