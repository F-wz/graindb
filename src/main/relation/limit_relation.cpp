#include "graindb/main/relation/limit_relation.hpp"
#include "graindb/parser/query_node/select_node.hpp"
#include "graindb/parser/query_node.hpp"
#include "graindb/parser/expression/constant_expression.hpp"

namespace graindb {

LimitRelation::LimitRelation(shared_ptr<Relation> child_p, int64_t limit, int64_t offset)
    : Relation(child_p->context, RelationType::PROJECTION_RELATION), limit(limit), offset(offset), child(move(child_p)) {
}

unique_ptr<QueryNode> LimitRelation::GetQueryNode() {
	auto child_node = child->GetQueryNode();
	auto limit_node = make_unique<LimitModifier>();
	if (limit >= 0) {
		limit_node->limit = make_unique<ConstantExpression>(SQLType::BIGINT, Value::BIGINT(limit));
	}
	if (offset > 0) {
		limit_node->offset = make_unique<ConstantExpression>(SQLType::BIGINT, Value::BIGINT(offset));
	}

	child_node->modifiers.push_back(move(limit_node));
	return child_node;
}

string LimitRelation::GetAlias() {
	return child->GetAlias();
}

const vector<ColumnDefinition> &LimitRelation::Columns() {
	return child->Columns();
}

string LimitRelation::ToString(idx_t depth) {
	string str = RenderWhitespace(depth) + "Limit " + std::to_string(limit);
	if (offset > 0) {
		str += " Offset " + std::to_string(offset);
	}
	str += "\n";
	return str + child->ToString(depth + 1);
}

} // namespace graindb
