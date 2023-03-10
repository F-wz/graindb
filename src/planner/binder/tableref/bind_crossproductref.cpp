#include "graindb/parser/tableref/crossproductref.hpp"
#include "graindb/planner/binder.hpp"
#include "graindb/planner/tableref/bound_crossproductref.hpp"

using namespace graindb;
using namespace std;

unique_ptr<BoundTableRef> Binder::Bind(CrossProductRef &ref) {
	auto result = make_unique<BoundCrossProductRef>();
	result->left = Bind(*ref.left);
	result->right = Bind(*ref.right);
	return move(result);
}
