#pragma once

#include "catch.hpp"
#include "graindb.hpp"
#include "graindb/optimizer/expression_rewriter.hpp"
#include "graindb/parser/parsed_expression.hpp"
#include "graindb/planner/expression.hpp"
#include "graindb/planner/planner.hpp"

namespace graindb {

class ClientContext;

class ExpressionHelper {
public:
	ExpressionHelper();

	unique_ptr<Expression> ParseExpression(string expression);
	unique_ptr<Expression> ApplyExpressionRule(unique_ptr<Expression> root);

	unique_ptr<LogicalOperator> ParseLogicalTree(string query);

	template <class T> void AddRule() {
		rewriter.rules.push_back(make_unique<T>(rewriter));
	}

	bool VerifyRewrite(string input, string expected_output, bool silent = false);

	string AddColumns(string columns);

	GrainDB db;
	Connection con;

private:
	ExpressionRewriter rewriter;

	string from_clause;
};

} // namespace graindb
