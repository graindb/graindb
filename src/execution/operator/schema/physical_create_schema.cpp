#include "duckdb/execution/operator/schema/physical_create_schema.hpp"

#include "duckdb/catalog/catalog.hpp"

using namespace duckdb;
using namespace std;

void PhysicalCreateSchema::GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state,
                                            SelectionVector *sel, Vector *rid_vector, DataChunk *rai_chunk) {
	Catalog::GetCatalog(context).CreateSchema(context, info.get());
	state->finished = true;
}
