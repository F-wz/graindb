#include "graindb/execution/operator/scan/physical_table_function.hpp"

#include "graindb/catalog/catalog_entry/schema_catalog_entry.hpp"
#include "graindb/catalog/catalog_entry/table_function_catalog_entry.hpp"
#include "graindb/execution/expression_executor.hpp"
#include "graindb/planner/expression/bound_function_expression.hpp"

using namespace graindb;
using namespace std;

void PhysicalTableFunction::GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
                                             SelectionVector *sel, Vector *rid_vector, DataChunk *rai_chunk) {
	// run main code
	function->function.function(context, parameters, chunk, bind_data.get());
	if (chunk.size() == 0) {
		// finished, call clean up
		if (function->function.final) {
			function->function.final(context, bind_data.get());
		}
	}
}
