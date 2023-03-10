#include "graindb/execution/operator/schema/physical_alter.hpp"

#include "graindb/main/client_context.hpp"
#include "graindb/parser/parsed_data/alter_table_info.hpp"

using namespace std;

namespace graindb {

void PhysicalAlter::GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state,
                                     SelectionVector *sel, Vector *rid_vector, DataChunk *rai_chunk) {
	auto table_info = (AlterTableInfo *)info.get();
	context.catalog.AlterTable(context, table_info);
	state->finished = true;
}

} // namespace graindb
