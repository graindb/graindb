#include "duckdb/storage/alist.hpp"

using namespace duckdb;
using namespace std;

void AList::AppendPKFK(Vector &source, Vector &edges, idx_t count, RAIDirection direction) {
	VectorData source_data, edges_data;
	source.Orrify(count, source_data);
	edges.Orrify(count, edges_data);
	auto source_ids = (int64_t *)source_data.data;
	auto edge_ids = (int64_t *)edges_data.data;
	for (idx_t i = 0; i < count; i++) {
		auto source_idx = source_data.sel->get_index(i);
		if ((*source_data.nullmask)[source_idx]) {
			continue;
		}
		auto source_rid = source_ids[source_idx];
		auto edge_rid = edge_ids[edges_data.sel->get_index(i)];
		if (forward_map.find(source_rid) == forward_map.end()) {
			auto elist = make_unique<EList>();
			forward_map[source_rid] = move(elist);
		}
		forward_map[source_rid]->edges->push_back(edge_rid);
		forward_map[source_rid]->size++;
	}
	edge_num += count;
}

void AList::Append(Vector &source, Vector &edges, Vector &target, idx_t count, RAIDirection direction) {
	VectorData source_data{}, edges_data{}, target_data{};
	source.Orrify(count, source_data);
	edges.Orrify(count, edges_data);
	target.Orrify(count, target_data);
	auto source_ids = (int64_t *)source_data.data;
	auto edge_ids = (int64_t *)edges_data.data;
	auto target_ids = (int64_t *)target_data.data;
	for (idx_t i = 0; i < count; i++) {
		auto source_idx = source_data.sel->get_index(i);
		auto target_idx = target_data.sel->get_index(i);
		if ((*source_data.nullmask)[source_idx] || (*target_data.nullmask)[target_idx]) {
			continue;
		}
		auto source_rid = source_ids[source_idx];
		auto edge_rid = edge_ids[edges_data.sel->get_index(i)];
		auto target_rid = target_ids[target_idx];
		if (forward_map.find(source_rid) == forward_map.end()) {
			auto elist = make_unique<EList>();
			forward_map[source_rid] = move(elist);
		}
		forward_map[source_rid]->edges->push_back(edge_rid);
		forward_map[source_rid]->vertices->push_back(target_rid);
		forward_map[source_rid]->size++;
	}
	if (direction == RAIDirection::SELF || direction == RAIDirection::UNDIRECTED) {
		for (idx_t i = 0; i < count; i++) {
			auto source_idx = source_data.sel->get_index(i);
			auto target_idx = target_data.sel->get_index(i);
			if ((*source_data.nullmask)[source_idx] || (*target_data.nullmask)[target_idx]) {
				continue;
			}
			auto source_rid = source_ids[source_idx];
			auto edge_rid = edge_ids[edges_data.sel->get_index(i)];
			auto target_rid = target_ids[target_idx];
			if (backward_map.find(target_rid) == backward_map.end()) {
				auto elist = make_unique<EList>();
				backward_map[target_rid] = move(elist);
			}
			backward_map[target_rid]->edges->push_back(edge_rid);
			backward_map[target_rid]->vertices->push_back(source_rid);
			backward_map[target_rid]->size++;
		}
	}
	edge_num += count;
}

void AList::Finalize(RAIDirection direction) {
	if (direction == RAIDirection::PKFK) {
		compact_forward_list.offsets = unique_ptr<idx_t[]>(new idx_t[source_num]);
		compact_forward_list.sizes = unique_ptr<idx_t[]>(new idx_t[source_num]);
		compact_forward_list.edges = unique_ptr<int64_t[]>(new int64_t[edge_num]);
		idx_t current_offset = 0;
		for (idx_t i = 0; i < source_num; i++) {
			if (forward_map.find(i) != forward_map.end()) {
				auto elist_size = forward_map[i]->size;
				memcpy((int64_t *)(compact_forward_list.edges.get()) + current_offset,
				       (int64_t *)(&forward_map[i]->edges->operator[](0)), elist_size * sizeof(int64_t));
				compact_forward_list.sizes[i] = elist_size;
				compact_forward_list.offsets[i] = current_offset;
				current_offset += elist_size;
			} else {
				compact_forward_list.sizes[i] = 0;
				compact_forward_list.offsets[i] = current_offset;
			}
		}
		forward_map.clear();
	} else {
		compact_forward_list.offsets = unique_ptr<idx_t[]>(new idx_t[source_num]);
		compact_forward_list.sizes = unique_ptr<idx_t[]>(new idx_t[source_num]);
		compact_forward_list.edges = unique_ptr<int64_t[]>(new int64_t[edge_num]);
		compact_forward_list.vertices = unique_ptr<int64_t[]>(new int64_t[edge_num]);
		idx_t current_offset = 0;
		for (idx_t i = 0; i < source_num; i++) {
			if (forward_map.find(i) != forward_map.end()) {
				auto elist_size = forward_map[i]->size;
				memcpy((int64_t *)(compact_forward_list.edges.get()) + current_offset,
				       (int64_t *)(&forward_map[i]->edges->operator[](0)), elist_size * sizeof(int64_t));
				memcpy((int64_t *)(compact_forward_list.vertices.get()) + current_offset,
				       (int64_t *)(&forward_map[i]->vertices->operator[](0)), elist_size * sizeof(int64_t));
				compact_forward_list.sizes[i] = elist_size;
				compact_forward_list.offsets[i] = current_offset;
				current_offset += elist_size;
			} else {
				compact_forward_list.sizes[i] = 0;
				compact_forward_list.offsets[i] = current_offset;
			}
		}
		if (backward_map.size() != 0) {
			compact_backward_list.offsets = unique_ptr<idx_t[]>(new idx_t[target_num]);
			compact_backward_list.sizes = unique_ptr<idx_t[]>(new idx_t[target_num]);
			compact_backward_list.edges = unique_ptr<int64_t[]>(new int64_t[edge_num]);
			compact_backward_list.vertices = unique_ptr<int64_t[]>(new int64_t[edge_num]);
			current_offset = 0;
			for (idx_t i = 0; i < target_num; i++) {
				if (backward_map.find(i) != backward_map.end()) {
					auto elist_size = backward_map[i]->size;
					memcpy((int64_t *)(compact_backward_list.edges.get()) + current_offset,
					       (int64_t *)(&backward_map[i]->edges->operator[](0)), elist_size * sizeof(int64_t));
					memcpy((int64_t *)(compact_backward_list.vertices.get()) + current_offset,
					       (int64_t *)(&backward_map[i]->vertices->operator[](0)), elist_size * sizeof(int64_t));
					compact_backward_list.sizes[i] = elist_size;
					compact_backward_list.offsets[i] = current_offset;
					current_offset += elist_size;
				} else {
					compact_backward_list.sizes[i] = 0;
					compact_backward_list.offsets[i] = current_offset;
				}
			}
		}
		forward_map.clear();
		backward_map.clear();
	}
	enabled = true;
	src_avg_degree = (double)edge_num / (double)source_num;
	dst_avg_degree = (double)edge_num / (double)target_num;
}

static idx_t FetchInternal(CompactList &alist, idx_t &lpos, idx_t &rpos, Vector &rvector, idx_t rsize,
                           SelectionVector &rsel, Vector &r0vector, Vector &r1vector) {
	// assert(r0vector.vector_type == VectorType::FLAT_VECTOR);
	// assert(r1vector.vector_type == VectorType::FLAT_VECTOR);
	if (rpos >= rsize) {
		return 0;
	}

	r0vector.vector_type = VectorType::FLAT_VECTOR;
	r1vector.vector_type = VectorType::FLAT_VECTOR;
	VectorData right_data;
	rvector.Orrify(rsize, right_data);
	auto rdata = (int64_t *)right_data.data;
	auto r0data = (int64_t *)FlatVector::GetData(r0vector);
	auto r1data = (int64_t *)FlatVector::GetData(r1vector);
	idx_t result_count = 0;

	if (right_data.nullmask->any()) {
		while (rpos < rsize) {
			idx_t right_position = right_data.sel->get_index(rpos);
			if ((*right_data.nullmask)[right_position]) {
				continue;
			}
			auto rid = rdata[right_position];
			auto offset = alist.offsets.operator[](rid);
			auto length = alist.sizes.operator[](rid);
			auto result_size = std::min(length - lpos, STANDARD_VECTOR_SIZE - result_count);
			memcpy(r0data + result_count, alist.edges.get() + (offset + lpos), result_size * sizeof(int64_t));
			memcpy(r1data + result_count, alist.vertices.get() + (offset + lpos), result_size * sizeof(int64_t));
			for (idx_t i = 0; i < result_size; i++) {
				rsel.set_index(result_count++, rpos);
			}
			lpos += result_size;
			if (lpos == length) {
				lpos = 0;
				rpos++;
			}
			if (result_count == STANDARD_VECTOR_SIZE) {
				return result_count;
			}
		}
	} else {
		while (rpos < rsize) {
			idx_t right_position = right_data.sel->get_index(rpos);
			auto rid = rdata[right_position];
			auto offset = alist.offsets.operator[](rid);
			auto length = alist.sizes.operator[](rid);
			auto result_size = std::min(length - lpos, STANDARD_VECTOR_SIZE - result_count);
			memcpy(r0data + result_count, alist.edges.get() + (offset + lpos), result_size * sizeof(int64_t));
			memcpy(r1data + result_count, alist.vertices.get() + (offset + lpos), result_size * sizeof(int64_t));
			for (idx_t i = 0; i < result_size; i++) {
				rsel.set_index(result_count++, rpos);
			}
			lpos += result_size;
			if (lpos == length) {
				lpos = 0;
				rpos++;
			}
			if (result_count == STANDARD_VECTOR_SIZE) {
				return result_count;
			}
		}
	}

	return result_count;
}

static idx_t FetchVertexesInternal(CompactList &alist, idx_t &lpos, idx_t &rpos, Vector &rvector, idx_t rsize,
                                   SelectionVector &rsel, Vector &r0vector) {
	assert(r0vector.vector_type == VectorType::FLAT_VECTOR);
	if (rpos >= rsize) {
		return 0;
	}

	VectorData right_data;
	rvector.Orrify(rsize, right_data);
	auto rdata = (int64_t *)right_data.data;
	auto r0data = (int64_t *)FlatVector::GetData(r0vector);
	idx_t result_count = 0;

	if (right_data.nullmask->any()) {
		while (rpos < rsize) {
			idx_t right_position = right_data.sel->get_index(rpos);
			if ((*right_data.nullmask)[right_position]) {
				continue;
			}
			auto rid = rdata[right_position];
			auto offset = alist.offsets.operator[](rid);
			auto length = alist.sizes.operator[](rid);
			auto result_size = std::min(length - lpos, STANDARD_VECTOR_SIZE - result_count);
			memcpy(r0data + result_count, alist.vertices.get() + (offset + lpos), result_size * sizeof(int64_t));
			for (idx_t i = 0; i < result_size; i++) {
				rsel.set_index(result_count++, rpos);
			}
			lpos += result_size;
			if (lpos == length) {
				lpos = 0;
				rpos++;
			}
			if (result_count == STANDARD_VECTOR_SIZE) {
				return result_count;
			}
		}
	} else {
		while (rpos < rsize) {
			idx_t right_position = right_data.sel->get_index(rpos);
			auto rid = rdata[right_position];
			auto offset = alist.offsets.operator[](rid);
			auto length = alist.sizes.operator[](rid);
			auto result_size = std::min(length - lpos, STANDARD_VECTOR_SIZE - result_count);
			memcpy(r0data + result_count, alist.vertices.get() + (offset + lpos), result_size * sizeof(int64_t));
			for (idx_t i = 0; i < result_size; i++) {
				rsel.set_index(result_count++, rpos);
			}
			lpos += result_size;
			if (lpos == length) {
				lpos = 0;
				rpos++;
			}
			if (result_count == STANDARD_VECTOR_SIZE) {
				return result_count;
			}
		}
	}
	return result_count;
}

idx_t AList::Fetch(idx_t &lpos, idx_t &rpos, Vector &rvector, idx_t rsize, SelectionVector &rsel, Vector &r0vector,
                   Vector &r1vector, bool forward) {
	if (forward) {
		return FetchInternal(compact_forward_list, lpos, rpos, rvector, rsize, rsel, r0vector, r1vector);
	}
	return FetchInternal(compact_backward_list, lpos, rpos, rvector, rsize, rsel, r0vector, r1vector);
}

idx_t AList::FetchVertexes(idx_t &lpos, idx_t &rpos, Vector &rvector, idx_t rsize, SelectionVector &rsel,
                           Vector &r0vector, bool forward) {
	if (forward) {
		return FetchVertexesInternal(compact_forward_list, lpos, rpos, rvector, rsize, rsel, r0vector);
	}
	return FetchVertexesInternal(compact_backward_list, lpos, rpos, rvector, rsize, rsel, r0vector);
}

static idx_t BuildZoneFilterInternal(CompactList &alist, data_ptr_t *hashmap, idx_t size, bitmask_vector &zone_filter) {
	idx_t matched_count = 0;
	for (idx_t i = 0; i < size; i++) {
		if (*((data_ptr_t *)(hashmap + i))) {
			auto offset = alist.offsets[i];
			auto length = alist.sizes[i];
			matched_count += length;
			for (idx_t idx = 0; idx < length; idx++) {
				auto edge_id = alist.edges[idx + offset];
				auto zone_id = edge_id / STANDARD_VECTOR_SIZE;
				auto index_in_zone = edge_id - (zone_id * STANDARD_VECTOR_SIZE);
				if (!zone_filter[zone_id]) {
					zone_filter[zone_id] = new bitset<STANDARD_VECTOR_SIZE>();
				}
				//				zone_filter[zone_id]->operator[](index_in_zone) = true;
			}
		}
	}
	return matched_count;
}

idx_t AList::BuildZoneFilter(data_ptr_t *hashmap, idx_t size, bitmask_vector &zone_filter, bool forward) {
	if (forward) {
		return BuildZoneFilterInternal(compact_forward_list, hashmap, size, zone_filter);
	}
	return BuildZoneFilterInternal(compact_backward_list, hashmap, size, zone_filter);
}

static idx_t BuildZoneFilterWithExtraInternal(CompactList &alist, data_ptr_t *hashmap, idx_t size,
                                              bitmask_vector &zone_filter, bitmask_vector &extra_zone_filter) {
	idx_t matched_count = 0;
	for (idx_t i = 0; i < size; i++) {
		if (*((data_ptr_t *)(hashmap + i))) {
			auto offset = alist.offsets[i];
			auto length = alist.sizes[i];
			matched_count += length;
			for (idx_t idx = 0; idx < length; idx++) {
				auto edge_id = alist.edges[idx + offset];
				auto zone_id = edge_id / STANDARD_VECTOR_SIZE;
				auto index_in_zone = edge_id - (zone_id * STANDARD_VECTOR_SIZE);
				if (!zone_filter[zone_id]) {
					zone_filter[zone_id] = new bitset<STANDARD_VECTOR_SIZE>();
				}
				//				zone_filter[zone_id]->operator[](index_in_zone) = true;
			}
			for (idx_t idx = 0; idx < length; idx++) {
				auto edge_id = alist.vertices[idx + offset];
				auto zone_id = edge_id / STANDARD_VECTOR_SIZE;
				auto index_in_zone = edge_id - (zone_id * STANDARD_VECTOR_SIZE);
				if (!extra_zone_filter[zone_id]) {
					extra_zone_filter[zone_id] = new bitset<STANDARD_VECTOR_SIZE>();
				}
				//				extra_zone_filter[zone_id]->operator[](index_in_zone) = true;
			}
		}
	}
	return matched_count;
}

idx_t AList::BuildZoneFilterWithExtra(data_ptr_t *hashmap, idx_t size, bitmask_vector &zone_filter,
                                      bitmask_vector &extra_zone_filter, bool forward) {
	if (forward) {
		return BuildZoneFilterWithExtraInternal(compact_forward_list, hashmap, size, zone_filter, extra_zone_filter);
	}
	return BuildZoneFilterWithExtraInternal(compact_backward_list, hashmap, size, zone_filter, extra_zone_filter);
}

static idx_t BuildRowsFilterInternal(CompactList &alist, data_ptr_t *hashmap, idx_t size, vector<row_t> &rows_filter) {
	idx_t matched_count = 0;
	for (idx_t i = 0; i < size; i++) {
		if (*((data_ptr_t *)(hashmap + i))) {
			auto offset = alist.offsets[i];
			auto length = alist.sizes[i];
			for (idx_t idx = 0; idx < length; idx++) {
				rows_filter[matched_count++] = alist.edges[idx + offset];
			}
		}
	}
	return matched_count;
}

idx_t AList::BuildRowsFilter(data_ptr_t *hashmap, idx_t size, vector<row_t> &rows_filter, bool forward) {
	if (forward) {
		return BuildRowsFilterInternal(compact_forward_list, hashmap, size, rows_filter);
	}
	return BuildRowsFilterInternal(compact_backward_list, hashmap, size, rows_filter);
}

static idx_t BuildRowsFilterWithExtraInternal(CompactList &alist, data_ptr_t *hashmap, idx_t size,
                                              vector<row_t> &rows_filter, vector<row_t> &extra_rows_filter) {
	idx_t matched_count = 0;
	idx_t extra_matched_count = 0;
	for (idx_t i = 0; i < size; i++) {
		if (*((data_ptr_t *)(hashmap + i))) {
			auto offset = alist.offsets[i];
			auto length = alist.sizes[i];
			for (idx_t idx = 0; idx < length; idx++) {
				rows_filter[matched_count++] = alist.edges[idx + offset];
			}
			for (idx_t idx = 0; idx < length; idx++) {
				extra_rows_filter[extra_matched_count++] = alist.vertices[idx + offset];
			}
		}
	}
	assert(matched_count == extra_matched_count);
	return matched_count;
}

idx_t AList::BuildRowsFilterWithExtra(data_ptr_t *hashmap, idx_t size, vector<row_t> &rows_filter,
                                      vector<row_t> &extra_rows_filter, bool forward) {
	if (forward) {
		return BuildRowsFilterWithExtraInternal(compact_forward_list, hashmap, size, rows_filter, extra_rows_filter);
	}
	return BuildRowsFilterWithExtraInternal(compact_backward_list, hashmap, size, rows_filter, extra_rows_filter);
}
