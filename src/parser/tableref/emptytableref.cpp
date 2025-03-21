#include "graindb/parser/tableref/emptytableref.hpp"

#include "graindb/common/serializer.hpp"

using namespace graindb;
using namespace std;

bool EmptyTableRef::Equals(const TableRef *other) const {
	return TableRef::Equals(other);
}

unique_ptr<TableRef> EmptyTableRef::Copy() {
	return make_unique<EmptyTableRef>();
}

void EmptyTableRef::Serialize(Serializer &serializer) {
	TableRef::Serialize(serializer);
}

unique_ptr<TableRef> EmptyTableRef::Deserialize(Deserializer &source) {
	return make_unique<EmptyTableRef>();
}
