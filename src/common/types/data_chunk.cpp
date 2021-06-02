#include "duckdb/common/types/data_chunk.hpp"

#include "duckdb/common/exception.hpp"
#include "duckdb/common/helper.hpp"
#include "duckdb/common/printer.hpp"
#include "duckdb/common/serializer.hpp"
#include "duckdb/common/types/null_value.hpp"
#include "duckdb/common/vector_operations/vector_operations.hpp"
#include "duckdb/common/unordered_map.hpp"

using namespace duckdb;
using namespace std;

DataChunk::DataChunk() : count(0) {
}

void DataChunk::InitializeEmpty(vector<TypeId> &types) {
	assert(types.size() > 0);
	for (idx_t i = 0; i < types.size(); i++) {
		data.emplace_back(Vector(types[i], nullptr));
	}
}

void DataChunk::Initialize(vector<TypeId> &types) {
	assert(types.size() > 0);
	InitializeEmpty(types);
	for (idx_t i = 0; i < types.size(); i++) {
		data[i].Initialize();
	}
}

void DataChunk::Reset() {
	for (idx_t i = 0; i < column_count(); i++) {
		data[i].Initialize();
	}
	SetCardinality(0);
}

void DataChunk::Destroy() {
	data.clear();
	SetCardinality(0);
}

Value DataChunk::GetValue(idx_t col_idx, idx_t index) const {
	assert(index < size());
	return data[col_idx].GetValue(index);
}

void DataChunk::SetValue(idx_t col_idx, idx_t index, Value val) {
	data[col_idx].SetValue(index, move(val));
}

idx_t DataChunk::GetZoneFilter(column_t column_idx, vector<bool> &zone_filter) {
	VectorData rid_data;
	data[column_idx].Orrify(count, rid_data);
	auto rid = (int64_t *)rid_data.data;
	for (idx_t i = 0; i < count; i++) {
		idx_t position = rid_data.sel->get_index(i);
		zone_filter[rid[position] / STANDARD_VECTOR_SIZE] = false;
	}
	return count;
}

void DataChunk::Reference(DataChunk &chunk) {
	assert(chunk.column_count() <= column_count());
	SetCardinality(chunk);
	for (idx_t i = 0; i < chunk.column_count(); i++) {
		data[i].Reference(chunk.data[i]);
	}
}

void DataChunk::Copy(DataChunk &other, idx_t offset) {
	assert(column_count() == other.column_count());
	assert(other.size() == 0);

	for (idx_t i = 0; i < column_count(); i++) {
		assert(other.data[i].vector_type == VectorType::FLAT_VECTOR);
		VectorOperations::Copy(data[i], other.data[i], size(), offset, 0);
	}
	other.SetCardinality(size() - offset);
}

void DataChunk::Append(DataChunk &other) {
	if (other.size() == 0) {
		return;
	}
	if (column_count() != other.column_count()) {
		throw OutOfRangeException("Column counts of appending chunk doesn't match!");
	}
	for (idx_t i = 0; i < column_count(); i++) {
		assert(data[i].vector_type == VectorType::FLAT_VECTOR);
		VectorOperations::Copy(other.data[i], data[i], other.size(), 0, size());
	}
	SetCardinality(size() + other.size());
}

void DataChunk::Normalify() {
	for (idx_t i = 0; i < column_count(); i++) {
		data[i].Normalify(size());
	}
}

vector<TypeId> DataChunk::GetTypes() {
	vector<TypeId> types;
	for (idx_t i = 0; i < column_count(); i++) {
		types.push_back(data[i].type);
	}
	return types;
}

string DataChunk::ToString() const {
	string retval = "Chunk - [" + to_string(column_count()) + " Columns]\n";
	for (idx_t i = 0; i < column_count(); i++) {
		retval += "- " + data[i].ToString(size()) + "\n";
	}
	return retval;
}

void DataChunk::Serialize(Serializer &serializer) {
	// write the count
	serializer.Write<sel_t>(size());
	serializer.Write<idx_t>(column_count());
	for (idx_t col_idx = 0; col_idx < column_count(); col_idx++) {
		// write the types
		serializer.Write<int>((int)data[col_idx].type);
	}
	// write the data
	for (idx_t col_idx = 0; col_idx < column_count(); col_idx++) {
		data[col_idx].Serialize(size(), serializer);
	}
}

void DataChunk::Deserialize(Deserializer &source) {
	auto rows = source.Read<sel_t>();
	idx_t column_count = source.Read<idx_t>();

	vector<TypeId> types;
	for (idx_t i = 0; i < column_count; i++) {
		types.push_back((TypeId)source.Read<int>());
	}
	Initialize(types);
	// now load the column data
	SetCardinality(rows);
	for (idx_t i = 0; i < column_count; i++) {
		data[i].Deserialize(rows, source);
	}
	Verify();
}

void DataChunk::Slice(const SelectionVector &sel_vector, idx_t count) {
	this->count = count;
	sel_cache_t merge_cache;
	for (idx_t c = 0; c < column_count(); c++) {
		data[c].Slice(sel_vector, count, merge_cache);
	}
}

void DataChunk::Slice(DataChunk &other, const SelectionVector &sel, idx_t count, idx_t col_offset) {
	assert(other.column_count() <= col_offset + column_count());  // todo: ? other.column_count + col_offset <= column_count()
	this->count = count;
	sel_cache_t merge_cache;
	for (idx_t c = 0; c < other.column_count(); c++) {
		if (other.data[c].vector_type == VectorType::DICTIONARY_VECTOR) {
			// already a dictionary! merge the dictionaries
			data[col_offset + c].Reference(other.data[c]);
			data[col_offset + c].Slice(sel, count, merge_cache);
		} else {
			data[col_offset + c].Slice(other.data[c], sel, count);
		}
	}
}

void DataChunk::Slice(DataChunk &other, const SelectionVector &sel, idx_t count, idx_t col_offset, idx_t column_num) {
	assert(other.column_count() <= col_offset + column_count());  // todo: ? other.column_count + col_offset <= column_count()
	this->count = count;
	sel_cache_t merge_cache;
	for (idx_t c = 0; c < column_num; c++) {
		if (other.data[c].vector_type == VectorType::DICTIONARY_VECTOR) {
			// already a dictionary! merge the dictionaries
			data[col_offset + c].Reference(other.data[c]);
			data[col_offset + c].Slice(sel, count, merge_cache);
		} else {
			data[col_offset + c].Slice(other.data[c], sel, count);
		}
	}
}

void DataChunk::Slice(DataChunk &other, const SelectionVector &sel, idx_t count, idx_t col_offset, vector<idx_t> &projection) {
//	assert(other.column_count() <= col_offset + column_count());  // todo: ? other.column_count + col_offset <= column_count()
	assert(col_offset + projection.size() <= column_count());
	this->count = count;
	sel_cache_t merge_cache;
	for (idx_t c = 0; c < projection.size(); c++) {
		idx_t o_c = projection[c];
		if (other.data[o_c].vector_type == VectorType::DICTIONARY_VECTOR) {
			// already a dictionary! merge the dictionaries
			data[col_offset + c].Reference(other.data[o_c]);
			data[col_offset + c].Slice(sel, count, merge_cache);
		} else {
			data[col_offset + c].Slice(other.data[o_c], sel, count);
		}
	}
}

unique_ptr<VectorData[]> DataChunk::Orrify() {
	auto orrified_data = unique_ptr<VectorData[]>(new VectorData[column_count()]);
	for (idx_t col_idx = 0; col_idx < column_count(); col_idx++) {
		data[col_idx].Orrify(size(), orrified_data[col_idx]);
	}
	return orrified_data;
}

void DataChunk::Hash(Vector &result) {
	assert(result.type == TypeId::HASH);
	VectorOperations::Hash(data[0], result, size());
	for (idx_t i = 1; i < column_count(); i++) {
		VectorOperations::CombineHash(result, data[i], size());
	}
}

void DataChunk::Verify() {
#ifdef DEBUG
	assert(size() <= STANDARD_VECTOR_SIZE);
	// verify that all vectors in this chunk have the chunk selection vector
	for (idx_t i = 0; i < column_count(); i++) {
		data[i].Verify(size());
	}
#endif
}

void DataChunk::Print() {
	Printer::Print(ToString());
}
