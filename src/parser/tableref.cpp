#include "graindb/parser/tableref.hpp"

#include "graindb/common/printer.hpp"
#include "graindb/common/serializer.hpp"
#include "graindb/parser/tableref/list.hpp"

using namespace graindb;
using namespace std;

void TableRef::Serialize(Serializer &serializer) {
	serializer.Write<TableReferenceType>(type);
	serializer.WriteString(alias);
}

//! Deserializes a blob back into an TableRef
unique_ptr<TableRef> TableRef::Deserialize(Deserializer &source) {
	auto type = source.Read<TableReferenceType>();
	auto alias = source.Read<string>();
	unique_ptr<TableRef> result;
	switch (type) {
	case TableReferenceType::BASE_TABLE:
		result = BaseTableRef::Deserialize(source);
		break;
	case TableReferenceType::CROSS_PRODUCT:
		result = CrossProductRef::Deserialize(source);
		break;
	case TableReferenceType::JOIN:
		result = JoinRef::Deserialize(source);
		break;
	case TableReferenceType::SUBQUERY:
		result = SubqueryRef::Deserialize(source);
		break;
	case TableReferenceType::TABLE_FUNCTION:
		result = TableFunctionRef::Deserialize(source);
		break;
	case TableReferenceType::EMPTY:
		result = EmptyTableRef::Deserialize(source);
		break;
	case TableReferenceType::EXPRESSION_LIST:
		result = ExpressionListRef::Deserialize(source);
		break;
	case TableReferenceType::CTE:
	case TableReferenceType::INVALID:
		return nullptr;
	}
	result->alias = alias;
	return result;
}

void TableRef::Print() {
	Printer::Print(ToString());
}
