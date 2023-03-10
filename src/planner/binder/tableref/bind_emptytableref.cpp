#include "graindb/parser/tableref/emptytableref.hpp"
#include "graindb/planner/binder.hpp"
#include "graindb/planner/tableref/bound_dummytableref.hpp"

using namespace graindb;
using namespace std;

unique_ptr<BoundTableRef> Binder::Bind(EmptyTableRef &ref) {
	return make_unique<BoundEmptyTableRef>(GenerateTableIndex());
}
