#include "graindb/common/vector_operations/vector_operations.hpp"
#include "graindb/execution/expression_executor.hpp"
#include "graindb/planner/expression/bound_conjunction_expression.hpp"
#include "graindb/execution/adaptive_filter.hpp"

#include <chrono>
#include <random>
#include <vector>

using namespace graindb;
using namespace std;
using namespace chrono;

struct ConjunctionState : public ExpressionState {
	ConjunctionState(Expression &expr, ExpressionExecutorState &root)
	    : ExpressionState(expr, root) {
        adaptive_filter = make_unique<AdaptiveFilter>(expr);
    }
     unique_ptr<AdaptiveFilter> adaptive_filter;
};

unique_ptr<ExpressionState> ExpressionExecutor::InitializeState(BoundConjunctionExpression &expr,
                                                                ExpressionExecutorState &root) {
	auto result = make_unique<ConjunctionState>(expr, root);
	for (auto &child : expr.children) {
		result->AddChild(child.get());
	}
	return move(result);
}

void ExpressionExecutor::Execute(BoundConjunctionExpression &expr, ExpressionState *state, const SelectionVector *sel,
                                 idx_t count, Vector &result) {
	// execute the children
	for (idx_t i = 0; i < expr.children.size(); i++) {
		Vector current_result(TypeId::BOOL);
		Execute(*expr.children[i], state->child_states[i].get(), sel, count, current_result);
		if (i == 0) {
			// move the result
			result.Reference(current_result);
		} else {
			Vector intermediate(TypeId::BOOL);
			// AND/OR together
			switch (expr.type) {
			case ExpressionType::CONJUNCTION_AND:
				VectorOperations::And(current_result, result, intermediate, count);
				break;
			case ExpressionType::CONJUNCTION_OR:
				VectorOperations::Or(current_result, result, intermediate, count);
				break;
			default:
				throw NotImplementedException("Unknown conjunction type!");
			}
			result.Reference(intermediate);
		}
	}
}

idx_t ExpressionExecutor::Select(BoundConjunctionExpression &expr, ExpressionState *state_, const SelectionVector *sel,
                                 idx_t count, SelectionVector *true_sel, SelectionVector *false_sel) {
	auto state = (ConjunctionState *)state_;

	if (expr.type == ExpressionType::CONJUNCTION_AND) {
		// get runtime statistics
		auto start_time = high_resolution_clock::now();

		const SelectionVector *current_sel = sel;
		idx_t current_count = count;
		idx_t false_count = 0;

		unique_ptr<SelectionVector> temp_true, temp_false;
		if (false_sel) {
			temp_false = make_unique<SelectionVector>(STANDARD_VECTOR_SIZE);
		}
		if (!true_sel) {
			temp_true = make_unique<SelectionVector>(STANDARD_VECTOR_SIZE);
			true_sel = temp_true.get();
		}
		for (idx_t i = 0; i < expr.children.size(); i++) {
			idx_t tcount =
			    Select(*expr.children[state->adaptive_filter->permutation[i]], state->child_states[state->adaptive_filter->permutation[i]].get(),
			           current_sel, current_count, true_sel, temp_false.get());
			idx_t fcount = current_count - tcount;
			if (fcount > 0 && false_sel) {
				// move failing tuples into the false_sel
				// tuples passed, move them into the actual result vector
				for (idx_t i = 0; i < fcount; i++) {
					false_sel->set_index(false_count++, temp_false->get_index(i));
				}
			}
			current_count = tcount;
			if (current_count == 0) {
				break;
			}
			if (current_count < count) {
				// tuples were filtered out: move on to using the true_sel to only evaluate passing tuples in subsequent
				// iterations
				current_sel = true_sel;
			}
		}

		// adapt runtime statistics
		auto end_time = high_resolution_clock::now();
		state->adaptive_filter->AdaptRuntimeStatistics(
		                       duration_cast<duration<double>>(end_time - start_time).count());
		return current_count;
	} else {
		// get runtime statistics
		auto start_time = high_resolution_clock::now();

		const SelectionVector *current_sel = sel;
		idx_t current_count = count;
		idx_t result_count = 0;

		unique_ptr<SelectionVector> temp_true, temp_false;
		if (true_sel) {
			temp_true = make_unique<SelectionVector>(STANDARD_VECTOR_SIZE);
		}
		if (!false_sel) {
			temp_false = make_unique<SelectionVector>(STANDARD_VECTOR_SIZE);
			false_sel = temp_false.get();
		}
		for (idx_t i = 0; i < expr.children.size(); i++) {
			idx_t tcount =
			    Select(*expr.children[state->adaptive_filter->permutation[i]], state->child_states[state->adaptive_filter->permutation[i]].get(),
			           current_sel, current_count, temp_true.get(), false_sel);
			if (tcount > 0) {
				if (true_sel) {
					// tuples passed, move them into the actual result vector
					for (idx_t i = 0; i < tcount; i++) {
						true_sel->set_index(result_count++, temp_true->get_index(i));
					}
				}
				// now move on to check only the non-passing tuples
				current_count -= tcount;
				current_sel = false_sel;
			}
		}

		// adapt runtime statistics
		auto end_time = high_resolution_clock::now();
		state->adaptive_filter->AdaptRuntimeStatistics(
		                      duration_cast<duration<double>>(end_time - start_time).count());
		return result_count;
	}
}
