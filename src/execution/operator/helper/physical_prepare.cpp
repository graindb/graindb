#include "duckdb/execution/operator/helper/physical_prepare.hpp"

#include "duckdb/catalog/catalog_entry/prepared_statement_catalog_entry.hpp"
#include "duckdb/main/client_context.hpp"

using namespace duckdb;
using namespace std;

void PhysicalPrepare::GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state,
                                       SelectionVector *sel, Vector *rid_vector, DataChunk *rai_chunk) {
	// create the catalog entry
	auto entry = make_unique<PreparedStatementCatalogEntry>(name, move(prepared));
	entry->catalog = &context.catalog;

	// now store plan in context
	auto &dependencies = entry->prepared->dependencies;
	if (!context.prepared_statements->CreateEntry(context.ActiveTransaction(), name, move(entry), dependencies)) {
		throw Exception("Failed to prepare statement");
	}
	state->finished = true;
}
