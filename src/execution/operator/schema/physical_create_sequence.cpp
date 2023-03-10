#include "graindb/execution/operator/schema/physical_create_sequence.hpp"

#include "graindb/catalog/catalog.hpp"

using namespace graindb;
using namespace std;

void PhysicalCreateSequence::GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state,
                                              SelectionVector *sel, Vector *rid_vector, DataChunk *rai_chunk) {
	Catalog::GetCatalog(context).CreateSequence(context, info.get());
	state->finished = true;
}
