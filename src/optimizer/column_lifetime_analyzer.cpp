#include "graindb/optimizer/column_lifetime_optimizer.hpp"
#include "graindb/planner/expression/bound_columnref_expression.hpp"
#include "graindb/planner/operator/logical_comparison_join.hpp"
#include "graindb/planner/operator/logical_delim_join.hpp"
#include "graindb/planner/operator/logical_filter.hpp"

using namespace graindb;
using namespace std;

void ColumnLifetimeAnalyzer::ExtractUnusedColumnBindings(vector<ColumnBinding> bindings,
                                                         column_binding_set_t &unused_bindings) {
	for (idx_t i = 0; i < bindings.size(); i++) {
		if (column_references.find(bindings[i]) == column_references.end()) {
			unused_bindings.insert(bindings[i]);
		}
	}
}

void ColumnLifetimeAnalyzer::GenerateProjectionMap(vector<ColumnBinding> bindings,
                                                   column_binding_set_t &unused_bindings,
                                                   vector<idx_t> &projection_map) {
	if (unused_bindings.size() == 0) {
		return;
	}
	// now iterate over the result bindings of the child
	for (idx_t i = 0; i < bindings.size(); i++) {
		// if this binding does not belong to the unused bindings, add it to the projection map
		if (unused_bindings.find(bindings[i]) == unused_bindings.end()) {
			projection_map.push_back(i);
		}
	}
	if (projection_map.size() == bindings.size()) {
		projection_map.clear();
	}
}

void ColumnLifetimeAnalyzer::StandardVisitOperator(LogicalOperator &op) {
	LogicalOperatorVisitor::VisitOperatorExpressions(op);
	if (op.type == LogicalOperatorType::DELIM_JOIN) {
		// visit the duplicate eliminated columns on the LHS, if any
		auto &delim_join = (LogicalDelimJoin &)op;
		for (auto &expr : delim_join.duplicate_eliminated_columns) {
			VisitExpression(&expr);
		}
	}
	LogicalOperatorVisitor::VisitOperatorChildren(op);
}

void ColumnLifetimeAnalyzer::VisitOperator(LogicalOperator &op) {
	switch (op.type) {
	case LogicalOperatorType::AGGREGATE_AND_GROUP_BY: {
		// FIXME: groups that are not referenced can be removed from projection
		// recurse into the children of the aggregate
		ColumnLifetimeAnalyzer analyzer;
		analyzer.column_ref_equivalence_cache = column_ref_equivalence_cache;
		analyzer.VisitOperatorExpressions(op);
		analyzer.VisitOperator(*op.children[0]);
		return;
	}
	case LogicalOperatorType::DELIM_JOIN:
	case LogicalOperatorType::COMPARISON_JOIN: {
		if (everything_referenced) {
			break;
		}
		auto &comp_join = (LogicalComparisonJoin &)op;
		if (comp_join.join_type == JoinType::MARK || comp_join.join_type == JoinType::SEMI ||
		    comp_join.join_type == JoinType::ANTI) {
			break;
		}
		// FIXME for now, we only push into the projection map for equality (hash) joins
		// FIXME: add projection to LHS as well
		bool has_equality = false;
		for (auto &cond : comp_join.conditions) {
			if (cond.comparison == ExpressionType::COMPARE_EQUAL) {
				has_equality = true;
				break;
			}
		}
		if (!has_equality) {
			break;
		}

		LogicalOperatorVisitor::VisitOperatorExpressions(op);
		if (op.type == LogicalOperatorType::DELIM_JOIN) {
			// visit the duplicate eliminated columns on the LHS, if any
			auto &delim_join = (LogicalDelimJoin &)op;
			for (auto &expr : delim_join.duplicate_eliminated_columns) {
				VisitExpression(&expr);
			}
		}

		// now, for each of the columns of the RHS, check which columns need to be projected
		column_binding_set_t unused_bindings;
		ExtractUnusedColumnBindings(op.children[1]->GetColumnBindings(), unused_bindings);

		// now recurse into the filter and its children
//		StandardVisitOperator(op);
		LogicalOperatorVisitor::VisitOperatorChildren(op);

		// then generate the projection map
		GenerateProjectionMap(op.children[1]->GetColumnBindings(), unused_bindings, comp_join.right_projection_map);
//		if (comp_join.op_mark == OpMark::MERGED_RJ) {
//			column_binding_set_t unused_merged_bindings;
//			ExtractUnusedColumnBindings(op.children[2]->GetColumnBindings(), unused_merged_bindings);
//			GenerateProjectionMap(op.children[2]->GetColumnBindings(), unused_merged_bindings,
//			                      comp_join.merge_projection_map);
//		}
		return;
	}
	case LogicalOperatorType::UNION:
	case LogicalOperatorType::EXCEPT:
	case LogicalOperatorType::INTERSECT:
		// for set operations we don't remove anything, just recursively visit the children
		// FIXME: for UNION we can remove unreferenced columns as long as everything_referenced is false (i.e. we
		// encounter a UNION node that is not preceded by a DISTINCT)
		for (auto &child : op.children) {
			ColumnLifetimeAnalyzer analyzer(true);
			analyzer.column_ref_equivalence_cache = column_ref_equivalence_cache;
			analyzer.VisitOperator(*child);
		}
		return;
	case LogicalOperatorType::PROJECTION: {
		// then recurse into the children of this projection
		ColumnLifetimeAnalyzer analyzer;
		analyzer.column_ref_equivalence_cache = column_ref_equivalence_cache;
		analyzer.VisitOperatorExpressions(op);
		analyzer.VisitOperator(*op.children[0]);
		return;
	}
	case LogicalOperatorType::DISTINCT: {
		// distinct, all projected columns are used for the DISTINCT computation
		// mark all columns as used and continue to the children
		// FIXME: DISTINCT with expression list does not implicitly reference everything
		everything_referenced = true;
		break;
	}
	case LogicalOperatorType::FILTER: {
		auto &filter = (LogicalFilter &)op;
		if (everything_referenced) {
			break;
		}
		// filter, figure out which columns are not needed after the filter
		column_binding_set_t unused_bindings;
		ExtractUnusedColumnBindings(op.children[0]->GetColumnBindings(), unused_bindings);

		// now recurse into the filter and its children
		StandardVisitOperator(op);

		// then generate the projection map
		GenerateProjectionMap(op.children[0]->GetColumnBindings(), unused_bindings, filter.projection_map);
		return;
	}
	case LogicalOperatorType::GET: {
		return;
	}
	default:
		break;
	}
	StandardVisitOperator(op);
}

unique_ptr<Expression> ColumnLifetimeAnalyzer::VisitReplace(BoundColumnRefExpression &expr,
                                                            unique_ptr<Expression> *expr_ptr) {
	for (auto &kv : column_ref_equivalence_cache) {
		if (kv.first == expr.binding) {
			auto replace_binding = kv.second;
			column_references.insert(replace_binding);
			return make_unique<BoundColumnRefExpression>(expr.alias, TypeId::INT64, replace_binding);
		}
	}
	column_references.insert(expr.binding);
	return nullptr;
}

unique_ptr<Expression> ColumnLifetimeAnalyzer::VisitReplace(BoundReferenceExpression &expr,
                                                            unique_ptr<Expression> *expr_ptr) {
	// BoundReferenceExpression should not be used here yet, they only belong in the physical plan
	throw InternalException("BoundReferenceExpression should not be used here yet!");
}
