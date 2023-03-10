#include "graindb/common/exception.hpp"
#include "graindb/parser/tableref.hpp"
#include "graindb/parser/transformer.hpp"

using namespace graindb;
using namespace std;

unique_ptr<TableRef> Transformer::TransformTableRefNode(PGNode *n) {
	switch (n->type) {
	case T_PGRangeVar:
		return TransformRangeVar(reinterpret_cast<PGRangeVar *>(n));
	case T_PGJoinExpr:
		return TransformJoin(reinterpret_cast<PGJoinExpr *>(n));
	case T_PGRangeSubselect:
		return TransformRangeSubselect(reinterpret_cast<PGRangeSubselect *>(n));
	case T_PGRangeFunction:
		return TransformRangeFunction(reinterpret_cast<PGRangeFunction *>(n));
	default:
		throw NotImplementedException("From Type %d not supported yet...", n->type);
	}
}
