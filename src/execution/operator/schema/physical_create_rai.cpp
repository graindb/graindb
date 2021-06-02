#include "duckdb/execution/operator/schema/physical_create_rai.hpp"

#include "duckdb/main/client_context.hpp"
#include "duckdb/storage/rai.hpp"

using namespace duckdb;
using namespace std;

void PhysicalCreateRAI::GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state,
                                         SelectionVector *sel, Vector *rid_vector, DataChunk *rai_chunk) {
	assert(children.size() == 1);
	auto rai = make_unique<RAI>(name, &table, rai_direction, column_ids, referenced_tables, referenced_columns);
	rai->alist->source_num = referenced_tables[0]->storage->info->cardinality;
	rai->alist->target_num = referenced_tables[1]->storage->info->cardinality;
	idx_t count = 0;
	auto append_states = unique_ptr<ColumnAppendState[]>(new ColumnAppendState[2]);
	vector<column_t> updated_columns = {0, 0};
	table.AddRAIColumns(column_ids, updated_columns);
	for (idx_t i = 0; i < 2; i++) {
		if (updated_columns[i] != 0) {
			table.storage->GetColumn(updated_columns[i])->InitializeAppend(append_states[i]);
		}
	}
	while (true) {
		children[0]->GetChunk(context, state->child_chunk, state->child_state.get());
		idx_t chunk_count = state->child_chunk.size();
		if (chunk_count == 0) {
			break;
		}
		if (updated_columns[0] != 0) {
			table.storage->GetColumn(updated_columns[0])
			    ->Append(append_states[0], state->child_chunk.data[0], chunk_count);
		}
		if (updated_columns[1] != 0) {
			table.storage->GetColumn(updated_columns[1])
			    ->Append(append_states[1], state->child_chunk.data[2], chunk_count);
		}
#if ENABLE_ALISTS
		if (rai->rai_direction == RAIDirection::PKFK) {
			rai->alist->AppendPKFK(state->child_chunk.data[0], state->child_chunk.data[1], chunk_count);
		} else {
			rai->alist->Append(state->child_chunk.data[0], state->child_chunk.data[1], state->child_chunk.data[2],
			                   chunk_count, rai_direction);
		}
#endif
		count += chunk_count;
	}
#if ENABLE_ALISTS
	rai->alist->Finalize(rai_direction);
#endif

	table.storage->AddRAI(move(rai));
	chunk.SetCardinality(1);
	chunk.SetValue(0, 0, Value::BIGINT(count));
	state->finished = true;
}
