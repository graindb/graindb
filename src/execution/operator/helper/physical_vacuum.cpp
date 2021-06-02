#include "duckdb/execution/operator/helper/physical_vacuum.hpp"

#include "duckdb/transaction/transaction.hpp"

using namespace duckdb;
using namespace std;

void PhysicalVacuum::GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state,
                                      SelectionVector *sel, Vector *rid_vector, DataChunk *rai_chunk) {
	auto vaccum_type = this->info->vaccum_type;
	// command analyze
	if (vaccum_type == PGVacuumOption::PG_VACOPT_ANALYZE) {
		// bind the cardinality with data table
		auto &transaction = Transaction::GetTransaction(context);
		Catalog::GetCatalog(context)
		    .GetSchema(context, DEFAULT_SCHEMA)
		    ->tables.Scan(transaction, [&](CatalogEntry *entry) {
			    auto table_catalog_entry = (TableCatalogEntry *)entry;
			    table_catalog_entry->storage->UpdateColumnCards(transaction);
		    });
	}
	chunk.SetCardinality(0);
	state->finished = true;
}
