#include "catch.hpp"
#include "graindb/common/helper.hpp"
#include "graindb/planner/expression/bound_operator_expression.hpp"
#include "graindb/optimizer/rule/like_optimizations.hpp"
#include "expression_helper.hpp"

using namespace graindb;
using namespace std;

TEST_CASE("Test Like Optimization Rules", "[like-optimizer]") {
	ExpressionHelper helper;

	REQUIRE(helper.AddColumns("S VARCHAR").empty());

	helper.AddRule<LikeOptimizationRule>();

	string input, expected_output;

	input = "S ~~ 'aaa%'";
	expected_output = "prefix(S, 'aaa')";
	REQUIRE(helper.VerifyRewrite(input, expected_output));

	input = "S ~~ '%aaa'";
	expected_output = "suffix(S, 'aaa')";
	REQUIRE(helper.VerifyRewrite(input, expected_output));

	input = "S ~~ '%aaa%'";
	expected_output = "contains(S, 'aaa')";
	REQUIRE(helper.VerifyRewrite(input, expected_output));

	// REQUIRE_FAIL ----------------
	input = "S ~~ 'a_a%'";
	expected_output = "prefix(S, 'aaa')";
	REQUIRE(helper.VerifyRewrite(input, expected_output, true) == false);

	input = "S ~~ '%a_a'";
	expected_output = "suffix(S, 'aaa')";
	REQUIRE(helper.VerifyRewrite(input, expected_output, true) == false);

	input = "S ~~ '%a_a%'";
	expected_output = "contains(S, 'a_a')";
	REQUIRE(helper.VerifyRewrite(input, expected_output, true) == false);
}
