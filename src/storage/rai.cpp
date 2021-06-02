#include "duckdb/storage/rai.hpp"

using namespace duckdb;
using namespace std;

void RAI::GetChunk(DataChunk &right_chunk, DataChunk &rid_chunk, DataChunk &new_chunk, idx_t &left_tuple,
                   idx_t &right_tuple, bool forward) const {
	assert(new_chunk.column_count() == right_chunk.column_count() + 2);
	SelectionVector rvector(STANDARD_VECTOR_SIZE);
	// join alist, get sel_vec and rai_chunk
	// in this context, l refers to alist, r refers to right_chunk/rid_chunk
	auto matched_count = alist->Fetch(left_tuple, right_tuple, rid_chunk.data[0], rid_chunk.size(), rvector,
	                                  new_chunk.data[right_chunk.column_count()],
	                                  new_chunk.data[right_chunk.column_count() + 1], forward);
	// slice and construct new_chunk
	new_chunk.Slice(right_chunk, rvector, matched_count);
	new_chunk.SetCardinality(matched_count);
}

void RAI::GetVertexes(DataChunk &right_chunk, DataChunk &rid_chunk, DataChunk &new_chunk, idx_t &left_tuple,
                      idx_t &right_tuple, bool forward) const {
	assert(new_chunk.column_count() == right_chunk.column_count() + 1);
	SelectionVector rvector(STANDARD_VECTOR_SIZE);
	auto matched_count = alist->FetchVertexes(left_tuple, right_tuple, rid_chunk.data[0], rid_chunk.size(), rvector,
	                                          new_chunk.data[right_chunk.column_count()], forward);
	// slice and construct new_chunk
	new_chunk.Slice(right_chunk, rvector, matched_count);
	new_chunk.SetCardinality(matched_count);
}

idx_t RAI::GetZoneFilter(data_ptr_t *hashmap, idx_t hm_size, shared_ptr<bitmask_vector> &zone_filter,
                         shared_ptr<bitmask_vector> &extra_zone_filter, bool forward) const {
	if (extra_zone_filter) {
		return alist->BuildZoneFilterWithExtra(hashmap, hm_size, *zone_filter, *extra_zone_filter, forward);
	} else {
		return alist->BuildZoneFilter(hashmap, hm_size, *zone_filter, forward);
	}
}

idx_t RAI::GetRowsFilter(data_ptr_t *hashmap, idx_t hm_size, shared_ptr<vector<row_t>> &rows_filter,
                         shared_ptr<vector<row_t>> &extra_rows_filter, bool forward) const {
	if (extra_rows_filter) {
		return alist->BuildRowsFilterWithExtra(hashmap, hm_size, *rows_filter, *extra_rows_filter, forward);
	} else {
		return alist->BuildRowsFilter(hashmap, hm_size, *rows_filter, forward);
	}
}
