#include "duckdb/execution/operator/scan/physical_empty_result.hpp"

using namespace duckdb;
using namespace std;

void PhysicalEmptyResult::GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state,
                                           SelectionVector *sel, Vector *rid_vector, DataChunk *rai_chunk) {
	state->finished = true;
}
