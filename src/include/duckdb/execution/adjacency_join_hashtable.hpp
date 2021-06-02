#pragma once

#include "duckdb/execution/rai_hashtable.hpp"

class AdjacencyJoinHashTable : public HashTable {
public:
	struct AdjacencyScanStructure : public ScanStructure {
		AdjacencyScanStructure(AdjacencyJoinHashTable &ht) : ScanStructure(ht) {
		}

		void Next(DataChunk &keys, DataChunk &left, DataChunk &result) override;
		void NextInnerJoin(DataChunk &keys, DataChunk &left, DataChunk &result);
		idx_t ScanInnerJoin(DataChunk &keys, SelectionVector &result_vector);
	};

	AdjacencyJoinHashTable(BufferManager &buffer_manager, vector<JoinCondition> &conditions, vector<TypeId> build_types,
	                       JoinType type);
	~AdjacencyJoinHashTable();

	//! Add the given data to the HT
	void Build(DataChunk &keys, DataChunk &input) override;
	//! Finalize the build of the HT, constructing the actual hash table and making the HT ready for probing. Finalize
	//! must be called before any call to Probe, and after Finalize is called Build should no longer be ever called.
	void Finalize() override;
	//! Probe the HT with the given input chunk, resulting in the given result
	unique_ptr<ScanStructure> Probe(DataChunk &keys) override;
};
