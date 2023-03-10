#include "graindb/parser/tableref/subqueryref.hpp"
#include "graindb/planner/binder.hpp"
#include "graindb/planner/tableref/bound_subqueryref.hpp"

using namespace graindb;
using namespace std;

unique_ptr<BoundTableRef> Binder::Bind(SubqueryRef &ref) {
	auto binder = make_unique<Binder>(context, this);
	auto subquery = binder->BindNode(*ref.subquery);
	idx_t bind_index = subquery->GetRootIndex();
	auto result = make_unique<BoundSubqueryRef>(move(binder), move(subquery));

	bind_context.AddSubquery(bind_index, ref.alias, ref, *result->subquery);
	MoveCorrelatedExpressions(*result->binder);
	return move(result);
}
