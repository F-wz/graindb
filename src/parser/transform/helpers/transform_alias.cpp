#include "graindb/parser/transformer.hpp"

using namespace graindb;
using namespace std;

string Transformer::TransformAlias(PGAlias *root) {
	if (!root) {
		return "";
	}
	return root->aliasname;
}
