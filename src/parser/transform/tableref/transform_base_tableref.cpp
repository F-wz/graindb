#include "graindb/parser/tableref/basetableref.hpp"
#include "graindb/parser/transformer.hpp"

using namespace graindb;
using namespace std;

unique_ptr<TableRef> Transformer::TransformRangeVar(PGRangeVar *root) {
	auto result = make_unique<BaseTableRef>();

	result->alias = TransformAlias(root->alias);
	if (root->relname)
		result->table_name = root->relname;
	if (root->schemaname)
		result->schema_name = root->schemaname;
	return move(result);
}
