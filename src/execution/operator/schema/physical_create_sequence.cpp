#include "duckdb/execution/operator/schema/physical_create_sequence.hpp"

#include "duckdb/catalog/catalog.hpp"

using namespace duckdb;
using namespace std;

void PhysicalCreateSequence::GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state,
                                              SelectionVector *sel, Vector *rid_vector, DataChunk *rai_chunk) {
	Catalog::GetCatalog(context).CreateSequence(context, info.get());
	state->finished = true;
}
