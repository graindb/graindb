#include "duckdb/execution/operator/helper/physical_execute.hpp"

using namespace duckdb;
using namespace std;

void PhysicalExecute::GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
                                       SelectionVector *sel, Vector *rid_vector, DataChunk *rai_chunk) {
	assert(plan);
	plan->GetChunk(context, chunk, state_, sel, rid_vector, rai_chunk);
}

unique_ptr<PhysicalOperatorState> PhysicalExecute::GetOperatorState() {
	return plan->GetOperatorState();
}
