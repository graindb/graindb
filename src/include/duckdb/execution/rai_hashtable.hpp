//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/execution/rai_hashtable.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/common.hpp"
#include "duckdb/common/file_buffer.hpp"
#include "duckdb/common/types/data_chunk.hpp"
#include "duckdb/common/types/vector.hpp"
#include "duckdb/execution/aggregate_hashtable.hpp"
#include "duckdb/planner/operator/logical_comparison_join.hpp"
#include "duckdb/storage/buffer/buffer_handle.hpp"
#include "duckdb/storage/storage_info.hpp"

namespace duckdb {

class RAIHashTable {
public:
	//! Scan structure that can be used to resume scans, as a single probe can
	//! return 1024*N values (where N is the size of the HT). This is
	//! returned by the RAIHashTable::Scan function and can be used to resume a
	//! probe.
	struct RAIScanStructure {
		unique_ptr<VectorData[]> key_data;
		Vector pointers;
		idx_t count;
		SelectionVector sel_vector;
		// whether or not the given tuple has found a match
		unique_ptr<bool[]> found_match;
		RAIHashTable &ht;
		bool finished;

		RAIScanStructure(RAIHashTable &ht);
		//! Get the next batch of data from the scan structure
		void Next(DataChunk &keys, DataChunk &left, DataChunk &result);

	private:
		void AdvancePointers();
		void AdvancePointers(const SelectionVector &sel, idx_t sel_count);

		//! Next operator for the inner join
		void NextInnerJoin(DataChunk &keys, DataChunk &left, DataChunk &result);
		//! Next operator for the semi join
		void NextSemiJoin(DataChunk &keys, DataChunk &left, DataChunk &result);
		//! Next operator for the anti join
		void NextAntiJoin(DataChunk &keys, DataChunk &left, DataChunk &result);
		//! Next operator for the left outer join
		void NextLeftJoin(DataChunk &keys, DataChunk &left, DataChunk &result);
		//! Next operator for the mark join
		void NextMarkJoin(DataChunk &keys, DataChunk &left, DataChunk &result);
		//! Next operator for the single join
		void NextSingleJoin(DataChunk &keys, DataChunk &left, DataChunk &result);

		//! Scan the hashtable for matches of the specified keys, setting the found_match[] array to true or false for
		//! every tuple
		void ScanKeyMatches(DataChunk &keys);
		template <bool MATCH> void NextSemiOrAntiJoin(DataChunk &keys, DataChunk &left, DataChunk &result);

		void ConstructMarkJoinResult(DataChunk &join_keys, DataChunk &child, DataChunk &result);

		idx_t ScanInnerJoin(DataChunk &keys, SelectionVector &result_vector);

		idx_t ResolvePredicates(DataChunk &keys, SelectionVector &match_sel);
		idx_t ResolvePredicates(DataChunk &keys, SelectionVector &match_sel, SelectionVector &no_match_sel);
		void GatherResult(Vector &result, const SelectionVector &result_vector, const SelectionVector &sel_vector,
		                  idx_t count, idx_t &offset);
		void GatherResult(Vector &result, const SelectionVector &sel_vector, idx_t count, idx_t &offset);

		template <bool NO_MATCH_SEL>
		idx_t ResolvePredicates(DataChunk &keys, SelectionVector *match_sel, SelectionVector *no_match_sel);
	};

private:
	//! Nodes store the actual data of the tuples inside the HT as a linked list
	struct HTDataBlock {
		idx_t count;
		idx_t capacity;
		block_id_t block_id;
	};

	idx_t AppendToBlock(HTDataBlock &block, BufferHandle &handle, idx_t count, data_ptr_t key_locations[],
	                    idx_t remaining);

public:
	RAIHashTable(BufferManager &buffer_manager, vector<JoinCondition> &conditions, vector<TypeId> build_types,
	             JoinType type, idx_t hashmap_size);
	~RAIHashTable();

	//! Initialize
	void Initialize();
	//! Add the given data to the HT
	void Build(DataChunk &keys, DataChunk &input);
	//! Probe the HT with the given input chunk, resulting in the given result
	unique_ptr<RAIScanStructure> Probe(DataChunk &keys);

	void GetZoneFilter(bitmask_vector &zone_filter);
	idx_t GetRowsFilter(vector<row_t> &rows_filter);
	data_ptr_t *GetHashmapBuffer() {
		return (data_ptr_t *)hash_map->node->buffer;
	}

	//! Copying not allowed
	RAIHashTable(const RAIHashTable &) = delete;

	idx_t size() {
		return count;
	}

	bool initialized;

	//! The stringheap of the RAIHashTable
	StringHeap string_heap;
	//! BufferManager
	BufferManager &buffer_manager;
	//! The types of the keys used in equality comparison
	vector<TypeId> equality_types;
	//! The types of the keys
	vector<TypeId> condition_types;
	//! The types of all conditions
	vector<TypeId> build_types;
	//! The comparison predicates
	vector<ExpressionType> predicates;
	//! Size of condition keys
	idx_t equality_size;
	//! Size of condition keys
	idx_t condition_size;
	//! Size of build tuple
	idx_t build_size;
	//! The size of an entry as stored in the HashTable
	idx_t entry_size;
	//! The total tuple size
	idx_t tuple_size;
	//! The join type of the HT
	JoinType join_type;
	//! Whether or not the HT has been finalized
	bool finalized;
	//! Whether or not any of the key elements contain NULL
	bool has_null;
	//! Bitmask for getting relevant bits from the hashes to determine the position
	uint64_t bitmask;
	//! The amount of entries stored per block
	idx_t block_capacity;

	idx_t build_hash_time = 0;
	idx_t build_serialize_keys_time = 0;
	idx_t build_serialize_payload_time = 0;
	idx_t build_serialize_hash_time = 0;
	idx_t probe_hash_time = 0;

	struct {
		//! The types of the duplicate eliminated columns, only used in correlated MARK JOIN for flattening ANY()/ALL()
		//! expressions
		vector<TypeId> correlated_types;
		//! The aggregate expression nodes used by the HT
		vector<unique_ptr<Expression>> correlated_aggregates;
		//! The HT that holds the group counts for every correlated column
		unique_ptr<SuperLargeHashTable> correlated_counts;
		//! Group chunk used for aggregating into correlated_counts
		DataChunk group_chunk;
		//! Payload chunk used for aggregating into correlated_counts
		DataChunk payload_chunk;
		//! Result chunk used for aggregating into correlated_counts
		DataChunk result_chunk;
	} correlated_mark_join_info;

private:
	idx_t PrepareKeys(DataChunk &keys, unique_ptr<VectorData[]> &key_data, const SelectionVector *&current_sel,
	                  SelectionVector &sel);
	void SerializeKey(VectorData &vdata, const SelectionVector &sel, idx_t count, data_ptr_t key_locations[]);
	void SerializeVectorData(VectorData &vdata, TypeId type, const SelectionVector &sel, idx_t count,
	                         data_ptr_t key_locations[]);
	void SerializeVector(Vector &v, idx_t vcount, const SelectionVector &sel, idx_t count, data_ptr_t key_locations[]);

	//! The amount of entries stored in the HT currently
	idx_t count;
	//! The blocks holding the main data of the hash table
	vector<HTDataBlock> blocks;
	//! Pinned handles, these are pinned during finalization only
	vector<unique_ptr<BufferHandle>> pinned_handles;
	//! The hash map of the HT, created after finalization
	unique_ptr<BufferHandle> hash_map;
	//! Whether or not NULL values are considered equal in each of the comparisons
	vector<bool> null_values_are_equal;
	//! The size of the hashmap
	idx_t hashmap_size;
};

} // namespace duckdb
