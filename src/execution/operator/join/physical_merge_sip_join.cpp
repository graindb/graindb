#include "duckdb/execution/operator/join/physical_merge_sip_join.hpp"

#include "duckdb/execution/operator/scan/physical_table_scan.hpp"

#include <iostream>

using namespace duckdb;
using namespace std;

class PhysicalMergeSIPJoinState : public PhysicalComparisonJoinState {
public:
	PhysicalMergeSIPJoinState(PhysicalOperator *left, PhysicalOperator *right, vector<JoinCondition> &conditions)
	    : PhysicalComparisonJoinState(left, right, conditions), initialized(false) {
	}

	bool initialized;
	DataChunk cached_chunk;
	DataChunk join_keys;
	ChunkCollection im_chunks;
	// state for SHJoin
	unique_ptr<SIPHashTable::SIPScanStructure> scan_structure;
	// state for NLAJoin
	DataChunk right_condition_chunk;
	idx_t right_chunk_idx;
	idx_t right_tuple;
	idx_t left_tuple;
};

PhysicalMergeSIPJoin::PhysicalMergeSIPJoin(ClientContext &context, LogicalOperator &op,
                                           unique_ptr<PhysicalOperator> left, unique_ptr<PhysicalOperator> right,
                                           vector<JoinCondition> cond, JoinType join_type,
                                           vector<idx_t> &left_projection_map, vector<idx_t> &right_projection_map,
                                           vector<idx_t> &merge_projection_map)
    : PhysicalComparisonJoin(op, PhysicalOperatorType::MERGE_SIP_JOIN, move(cond), join_type), hash_table(nullptr),
      right_projection_map(right_projection_map) {
	children.push_back(move(left));
	children.push_back(move(right));

	assert(left_projection_map.size() == 0);

	hash_table =
	    make_unique<SIPHashTable>(BufferManager::GetBufferManager(context), conditions,
	                              LogicalOperator::MapTypes(children[1]->GetTypes(), right_projection_map), type);
}

void PhysicalMergeSIPJoin::InitializeAList() {
	auto &rai_info = conditions[0].rais[0];
	// determine the alist for usage
	switch (rai_info->rai_type) {
	case RAIType::TARGET_EDGE: {
		rai_info->forward = true;
		rai_info->compact_list = &rai_info->rai->alist->compact_forward_list;
		break;
	}
	case RAIType::SOURCE_EDGE: {
		rai_info->forward = false;
		rai_info->compact_list = &rai_info->rai->alist->compact_backward_list;
		break;
	}
	default:
		break;
	}
}

void PhysicalMergeSIPJoin::InitializeZoneFilter() {
	auto &rai_info = conditions[0].rais[0];
	auto zone_size = (rai_info->left_cardinalities[0] / STANDARD_VECTOR_SIZE) + 1;
	rai_info->row_bitmask = make_unique<bitmask_vector>(zone_size * STANDARD_VECTOR_SIZE);
	rai_info->zone_bitmask = make_unique<bitmask_vector>(zone_size);
	if (rai_info->passing_tables[1] != 0) {
		auto extra_zone_size = (rai_info->left_cardinalities[1] / STANDARD_VECTOR_SIZE) + 1;
		rai_info->extra_row_bitmask = make_unique<bitmask_vector>(extra_zone_size * STANDARD_VECTOR_SIZE);
		rai_info->extra_zone_bitmask = make_unique<bitmask_vector>(extra_zone_size);
	}
}

void PhysicalMergeSIPJoin::ProbeHashTable(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_) {
	auto state = reinterpret_cast<PhysicalMergeSIPJoinState *>(state_);
	if (state->child_chunk.size() > 0 && state->scan_structure) {
		// still have elements remaining from the previous probe (i.e. we got
		// >1024 elements in the previous probe)
		state->scan_structure->Next(state->join_keys, state->child_chunk, chunk);
		if (chunk.size() > 0) {
			return;
		}
		state->scan_structure = nullptr;
	}

	// probe the HT
	do {
		// fetch the chunk from the left side
		children[0]->GetChunk(context, state->child_chunk, state->child_state.get());
		if (state->child_chunk.size() == 0) {
			return;
		}
		// remove any selection vectors
		if (hash_table->size() == 0) {
			// empty hash table, special case
			if (hash_table->join_type == JoinType::ANTI) {
				// anti join with empty hash table, NOP join
				// return the input
				assert(chunk.column_count() == state->child_chunk.column_count());
				chunk.Reference(state->child_chunk);
				return;
			} else if (hash_table->join_type == JoinType::MARK) {
				// MARK join with empty hash table
				assert(hash_table->join_type == JoinType::MARK);
				assert(chunk.column_count() == state->child_chunk.column_count() + 1);
				auto &result_vector = chunk.data.back();
				assert(result_vector.type == TypeId::BOOL);
				// for every data vector, we just reference the child chunk
				chunk.SetCardinality(state->child_chunk);
				for (idx_t i = 0; i < state->child_chunk.column_count(); i++) {
					chunk.data[i].Reference(state->child_chunk.data[i]);
				}
				// for the MARK vector:
				// if the HT has no NULL values (i.e. empty result set), return a vector that has false for every input
				// entry if the HT has NULL values (i.e. result set had values, but all were NULL), return a vector that
				// has NULL for every input entry
				if (!hash_table->has_null) {
					auto bool_result = FlatVector::GetData<bool>(result_vector);
					for (idx_t i = 0; i < chunk.size(); i++) {
						bool_result[i] = false;
					}
				} else {
					FlatVector::Nullmask(result_vector).set();
				}
				return;
			} else if (hash_table->join_type == JoinType::LEFT || hash_table->join_type == JoinType::OUTER ||
			           hash_table->join_type == JoinType::SINGLE) {
				// LEFT/FULL OUTER/SINGLE join and build side is empty
				// for the LHS we reference the data
				chunk.SetCardinality(state->child_chunk.size());
				for (idx_t i = 0; i < state->child_chunk.column_count(); i++) {
					chunk.data[i].Reference(state->child_chunk.data[i]);
				}
				// for the RHS
				for (idx_t k = state->child_chunk.column_count(); k < chunk.column_count(); k++) {
					chunk.data[k].vector_type = VectorType::CONSTANT_VECTOR;
					ConstantVector::SetNull(chunk.data[k], true);
				}
				return;
			}
		}
		// resolve the join keys for the left chunk
		state->lhs_executor.Execute(state->child_chunk, state->join_keys);

		// perform the actual probe
		state->scan_structure = hash_table->Probe(state->join_keys);
		state->scan_structure->Next(state->join_keys, state->child_chunk, chunk);
	} while (chunk.size() == 0);
}

void PhysicalMergeSIPJoin::PassZoneFilter() {
	// actually do the pushdown
	auto &rai_info = conditions[0].rais[0];
	children[0]->PushdownZoneFilter(rai_info->passing_tables[0], rai_info->row_bitmask, rai_info->zone_bitmask);
	if (rai_info->passing_tables[1] != 0) {
		children[0]->PushdownZoneFilter(rai_info->passing_tables[1], rai_info->extra_row_bitmask,
		                                rai_info->extra_zone_bitmask);
	}
}

void PhysicalMergeSIPJoin::AppendHTBlocks(PhysicalOperatorState *state_, DataChunk &chunk, DataChunk &build_chunk) {
	auto state = reinterpret_cast<PhysicalMergeSIPJoinState *>(state_);
	state->join_keys.SetCardinality(chunk);
	state->join_keys.data[0].Reference(chunk.data[chunk.column_count() - 1]);
	//	state->rhs_executor.Execute(chunk, state->join_keys);
	if (right_projection_map.size() > 0) {
		//		build_chunk.Reset();
		build_chunk.SetCardinality(chunk);
		for (idx_t i = 0; i < right_projection_map.size(); i++) {
			build_chunk.data[i].Reference(chunk.data[right_projection_map[i]]);
		}
	} else {
		//		build_chunk.Reset();
		build_chunk.SetCardinality(chunk);
		for (idx_t i = 0; i < build_chunk.column_count(); i++) {
			build_chunk.data[i].Reference(chunk.data[i]);
		}
	}
	hash_table->Build(state->join_keys, build_chunk);
}

void PhysicalMergeSIPJoin::PerformSHJoin(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_) {
	auto state = reinterpret_cast<PhysicalMergeSIPJoinState *>(state_);
	do {
		ProbeHashTable(context, chunk, state);
#if STANDARD_VECTOR_SIZE >= 128
		if (chunk.size() == 0) {
			if (state->cached_chunk.size() > 0) {
				// finished probing but cached data remains, return cached chunk
				chunk.Reference(state->cached_chunk);
				state->cached_chunk.Reset();
			}
			return;
		} else if (chunk.size() < 64) {
			// small chunk: add it to chunk cache and continue
			state->cached_chunk.Append(chunk);
			if (state->cached_chunk.size() >= (STANDARD_VECTOR_SIZE - 64)) {
				// chunk cache full: return it
				chunk.Reference(state->cached_chunk);
				state->cached_chunk.Reset();
				return;
			} else {
				// chunk cache not full: probe again
				chunk.Reset();
			}
		} else {
			return;
		}
#else
		return;
#endif
	} while (true);
}

void PhysicalMergeSIPJoin::GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
                                            SelectionVector *sel, Vector *rid_vector, DataChunk *rai_chunk) {
	auto state = reinterpret_cast<PhysicalMergeSIPJoinState *>(state_);
	if (!state->initialized) {
		state->cached_chunk.Initialize(types);

		idx_t build_side_size = 0;
		auto right_state = children[1]->GetOperatorState();
		DataChunk right_chunk, build_chunk, im_chunk; // im_chunk: intermediate_chunk, [right_chunk.columns, tid]
		right_chunk.Initialize(children[1]->GetTypes());
		auto im_types = right_chunk.GetTypes();
		im_types.push_back(TypeId::INT64);
		im_chunk.Initialize(im_types);
		build_chunk.InitializeEmpty(hash_table->build_types);
		state->join_keys.InitializeEmpty(hash_table->condition_types);
		state->right_condition_chunk.InitializeEmpty(hash_table->condition_types);
		// initialize alist pointer
		// InitializeAList();
		auto &rai_info = conditions[0].rais[0];
		while (true) {
			children[1]->GetChunk(context, right_chunk, right_state.get());
			state->rhs_executor.Execute(right_chunk, state->right_condition_chunk);
			state->right_tuple = 0;
			state->left_tuple = 0;
			if (right_chunk.size() == 0) {
				break;
			}
			build_side_size += right_chunk.size();
			do {
				rai_info->rai->GetVertexes(right_chunk, state->right_condition_chunk, im_chunk, state->left_tuple,
				                           state->right_tuple, rai_info->forward);
				AppendHTBlocks(state_, im_chunk, build_chunk);
			} while (state->right_tuple < right_chunk.size());
		}
		hash_table->Finalize();
		state->right_tuple = 0;
		state->left_tuple = 0;

		// estimate distinct build side keys as non empty hash slots num, assume no conflicts in each slot.
		idx_t non_empty_hash_slots = 0;
		auto pointers = (data_ptr_t *)hash_table->hash_map->node->buffer;
		for (idx_t i = 0; i < hash_table->bitmask; i++) {
			non_empty_hash_slots += (pointers[i] != nullptr);
		}

		// estimate semi-join filter passing ratio
		double filter_passing_ratio = (double)non_empty_hash_slots / (double)rai_info->left_cardinalities[0];
		if (filter_passing_ratio <= SIPJoin::SHJ_MAGIC) {
			// if passing ratio is low, generate and pass semi-join filter
			hash_table->GenerateBitmaskFilter(*rai_info, false);
			PassZoneFilter();
		}
		state->initialized = true;

		if (hash_table->size() == 0 && state->im_chunks.count == 0 &&
		    (hash_table->join_type == JoinType::INNER || hash_table->join_type == JoinType::SEMI)) {
			// empty hash table with INNER or SEMI join means empty result set
			return;
		}
	}

	// perform SHJ
	PerformSHJoin(context, chunk, state);
}

unique_ptr<PhysicalOperatorState> PhysicalMergeSIPJoin::GetOperatorState() {
	return make_unique<PhysicalMergeSIPJoinState>(children[0].get(), children[1].get(), conditions);
}

string PhysicalMergeSIPJoin::ExtraRenderInformation() const {
	string extra_info = "MERGE_SIP_JOIN";
	extra_info += JoinTypeToString(type);
	extra_info += ", build: ";
	extra_info +=
	    to_string(right_projection_map.size() == 0 ? children[1]->GetTypes().size() : right_projection_map.size());
	extra_info += "]\n";
	for (auto &it : conditions) {
		string op = ExpressionTypeToOperator(it.comparison);
		extra_info += it.left->GetName() + op + it.right->GetName() + "\n";
		//		for (auto &rai : it.rais) {
		//			extra_info += rai->ToString() + "\n";
		//		}
	}
	auto &rai_info = conditions[0].rais[0];
	extra_info += rai_info->ToString() + "\n";

	return extra_info;
}
