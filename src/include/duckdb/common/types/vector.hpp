//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/common/types/vector.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/bitset.hpp"
#include "duckdb/common/common.hpp"
#include "duckdb/common/enums/vector_type.hpp"
#include "duckdb/common/types/selection_vector.hpp"
#include "duckdb/common/types/value.hpp"
#include "duckdb/common/types/vector_buffer.hpp"

namespace duckdb {
//! Type used for nullmasks
typedef bitset<STANDARD_VECTOR_SIZE> nullmask_t;

//! Zero NULL mask: filled with the value 0 [READ ONLY]
extern nullmask_t ZERO_MASK;

struct VectorData {
	const SelectionVector *sel;
	data_ptr_t data;
	nullmask_t *nullmask;
};

class VectorStructBuffer;
class VectorListBuffer;
class ChunkCollection;

//!  Vector of values of a specified TypeId.
// Instead of the shared selection vector, we introduce a new vector type: DICTIONARY_VECTOR. A dictionary vector is
// essentially a vector with a selection vector: buffer holds a shared pointer to a SelectionVector object, and
// auxiliary holds a pointer to the child vector. That child vector can be any vector type again, e.g. we can have a
// DICTIONARY_VECTOR pointing to a SEQUENCE_VECTOR, or a DICTIONARY_VECTOR pointing to a FLAT_VECTOR, etc.
//
// Dictionaries are created using the Vector::Slice method (or the DataChunk::Slice method, which just calls
// Vector::Slice on all the children with a cache).
// If Slice is called on a vector that is already a DICTIONARY_VECTOR, the dictionaries are merged together to avoid
// nesting dictionaries. As multiple vectors might share the same dictionary (e.g. because they all passed through the
// same FILTER), there is also a cache utilized here that avoids performing the same dictionary merge multiple times.
// That means that if we call DataChunk::Slice on a chunk where all vectors have the same dictionary, the actual merging
// of dictionaries is only performed once rather than once per vector. The rest of the vectors simply obtain a shared
// pointer to the merged selection vector.
//
// One of the problems with having every vector have their own selection vector is potential code explosion, as now all
// input vectors can potentially have or not have a selection vector. To avoid code explosion, we introduce the
// Vector::Orrify method. This method takes a vector, and fills in the following VectorData object:
// struct VectorData {
//       const SelectionVector *sel;
//       data_ptr_t data;
//       nullmask_t *nullmask;
//};
//
// Note that the sel and nullmask, despite being both pointers, are always set. That means that if you call Orrify on
// e.g. a vector of type INT32, you can always loop over it like this regardless of vector type.
// example ungrouped SUM implementation:
// VectorData vdata;
// vector.Orrify(count, vdata);
// auto integers = (int32_t*) vdata.data;
// for(idx_t i = 0; i < count; i++) {
//   auto idx = vdata.sel->get_index(i);
//   sum += integers[idx];
//}
class Vector {
	friend struct ConstantVector;
	friend struct DictionaryVector;
	friend struct FlatVector;
	friend struct ListVector;
	friend struct StringVector;
	friend struct StructVector;
	friend struct SequenceVector;

	friend class DataChunk;

public:
	Vector();
	//! Create a vector of size one holding the passed on value
	Vector(Value value);
	//! Create an empty standard vector with a type, equivalent to calling Vector(type, true, false)
	Vector(TypeId type);
	//! Create a non-owning vector that references the specified data
	Vector(TypeId type, data_ptr_t dataptr);
	//! Create an owning vector that holds at most STANDARD_VECTOR_SIZE entries.
	/*!
	    Create a new vector
	    If create_data is true, the vector will be an owning empty vector.
	    If zero_data is true, the allocated data will be zero-initialized.
	*/
	Vector(TypeId type, bool create_data, bool zero_data);
	// implicit copying of Vectors is not allowed
	Vector(const Vector &) = delete;
	// but moving of vectors is allowed
	Vector(Vector &&other) noexcept;

	//! The vector type specifies how the data of the vector is physically stored (i.e. if it is a single repeated
	//! constant, if it is compressed)
	VectorType vector_type;
	//! The type of the elements stored in the vector (e.g. integer, float)
	TypeId type;

public:
	//! Create a vector that references the specified value.
	void Reference(Value &value);
	//! Causes this vector to reference the data held by the other vector.
	void Reference(Vector &other);

	//! Creates a reference to a slice of the other vector
	void Slice(Vector &other, idx_t offset);
	//! Creates a reference to a slice of the other vector
	void Slice(Vector &other, const SelectionVector &sel, idx_t count);
	//! Turns the vector into a dictionary vector with the specified dictionary
	void Slice(const SelectionVector &sel, idx_t count);
	//! Slice the vector, keeping the result around in a cache or potentially using the cache instead of slicing
	void Slice(const SelectionVector &sel, idx_t count, sel_cache_t &cache);

	//! Creates the data of this vector with the specified type. Any data that
	//! is currently in the vector is destroyed.
	void Initialize(TypeId new_type = TypeId::INVALID, bool zero_data = false);

	//! Converts this Vector to a printable string representation
	string ToString(idx_t count) const;
	void Print(idx_t count);

	string ToString() const;
	void Print();

	//! Flatten the vector, removing any compression and turning it into a FLAT_VECTOR
	void Normalify(idx_t count);
	void Normalify(const SelectionVector &sel, idx_t count);
	//! Obtains a selection vector and data pointer through which the data of this vector can be accessed
	void Orrify(idx_t count, VectorData &data);

	//! Turn the vector into a sequence vector
	void Sequence(int64_t start, int64_t increment);

	//! Verify that the Vector is in a consistent, not corrupt state. DEBUG
	//! FUNCTION ONLY!
	void Verify(idx_t count);
	void Verify(const SelectionVector &sel, idx_t count);
	void UTFVerify(idx_t count);
	void UTFVerify(const SelectionVector &sel, idx_t count);

	//! Returns the [index] element of the Vector as a Value.
	Value GetValue(idx_t index) const;
	//! Sets the [index] element of the Vector to the specified Value.
	void SetValue(idx_t index, Value val);

	//! Serializes a Vector to a stand-alone binary blob
	void Serialize(idx_t count, Serializer &serializer);
	//! Deserializes a blob back into a Vector
	void Deserialize(idx_t count, Deserializer &source);

protected:
	//! A pointer to the data.
	data_ptr_t data;
	//! The nullmask of the vector
	nullmask_t nullmask;
	//! The main buffer holding the data of the vector
	buffer_ptr<VectorBuffer> buffer;
	//! The buffer holding auxiliary data of the vector
	//! e.g. a string vector uses this to store strings
	buffer_ptr<VectorBuffer> auxiliary;
};

//! The DictionaryBuffer holds a selection vector
class VectorChildBuffer : public VectorBuffer {
public:
	VectorChildBuffer() : VectorBuffer(VectorBufferType::VECTOR_CHILD_BUFFER), data() {
	}

public:
	Vector data;
};

struct ConstantVector {
	static inline data_ptr_t GetData(Vector &vector) {
		assert(vector.vector_type == VectorType::CONSTANT_VECTOR || vector.vector_type == VectorType::FLAT_VECTOR);
		return vector.data;
	}
	template <class T> static inline T *GetData(Vector &vector) {
		return (T *)ConstantVector::GetData(vector);
	}
	static inline bool IsNull(const Vector &vector) {
		assert(vector.vector_type == VectorType::CONSTANT_VECTOR);
		return vector.nullmask[0];
	}
	static inline void SetNull(Vector &vector, bool is_null) {
		assert(vector.vector_type == VectorType::CONSTANT_VECTOR);
		vector.nullmask[0] = is_null;
	}
	static inline nullmask_t &Nullmask(Vector &vector) {
		assert(vector.vector_type == VectorType::CONSTANT_VECTOR);
		return vector.nullmask;
	}

	static const sel_t zero_vector[STANDARD_VECTOR_SIZE];
	static const SelectionVector ZeroSelectionVector;
};

struct DictionaryVector {
	static inline SelectionVector &SelVector(const Vector &vector) {
		assert(vector.vector_type == VectorType::DICTIONARY_VECTOR);
		return ((DictionaryBuffer &)*vector.buffer).GetSelVector();
	}
	static inline Vector &Child(const Vector &vector) {
		assert(vector.vector_type == VectorType::DICTIONARY_VECTOR);
		return ((VectorChildBuffer &)*vector.auxiliary).data;
	}
};

struct FlatVector {
	static inline data_ptr_t GetData(Vector &vector) {
		return ConstantVector::GetData(vector);
	}
	template <class T> static inline T *GetData(Vector &vector) {
		return ConstantVector::GetData<T>(vector);
	}
	static inline void SetData(Vector &vector, data_ptr_t data) {
		assert(vector.vector_type == VectorType::FLAT_VECTOR);
		vector.data = data;
	}
	template <class T> static inline T GetValue(Vector &vector, idx_t idx) {
		assert(vector.vector_type == VectorType::FLAT_VECTOR);
		return FlatVector::GetData<T>(vector)[idx];
	}
	static inline nullmask_t &Nullmask(Vector &vector) {
		assert(vector.vector_type == VectorType::FLAT_VECTOR);
		return vector.nullmask;
	}
	static inline void SetNullmask(Vector &vector, nullmask_t new_mask) {
		assert(vector.vector_type == VectorType::FLAT_VECTOR);
		vector.nullmask = move(new_mask);
	}
	static inline void SetNull(Vector &vector, idx_t idx, bool value) {
		assert(vector.vector_type == VectorType::FLAT_VECTOR);
		vector.nullmask[idx] = value;
	}
	static inline bool IsNull(const Vector &vector, idx_t idx) {
		assert(vector.vector_type == VectorType::FLAT_VECTOR);
		return vector.nullmask[idx];
	}

	static const sel_t incremental_vector[STANDARD_VECTOR_SIZE];
	static sel_t mutable_incremental_vector[STANDARD_VECTOR_SIZE];
	static const SelectionVector IncrementalSelectionVector;
};

struct ListVector {
	static ChunkCollection &GetEntry(const Vector &vector);
	static bool HasEntry(const Vector &vector);
	static void SetEntry(Vector &vector, unique_ptr<ChunkCollection> entry);
};

struct StringVector {
	//! Add a string to the string heap of the vector (auxiliary data)
	static string_t AddString(Vector &vector, const char *data, idx_t len);
	//! Add a string to the string heap of the vector (auxiliary data)
	static string_t AddString(Vector &vector, const char *data);
	//! Add a string to the string heap of the vector (auxiliary data)
	static string_t AddString(Vector &vector, string_t data);
	//! Add a string to the string heap of the vector (auxiliary data)
	static string_t AddString(Vector &vector, const string &data);
	//! Add a blob to the string heap of the vector (auxiliary data)
	static string_t AddBlob(Vector &vector, string_t data);
	//! Allocates an empty string of the specified size, and returns a writable pointer that can be used to store the
	//! result of an operation
	static string_t EmptyString(Vector &vector, idx_t len);

	//! Add a reference from this vector to the string heap of the provided vector
	static void AddHeapReference(Vector &vector, Vector &other);
};

struct StructVector {
	static bool HasEntries(const Vector &vector);
	static child_list_t<unique_ptr<Vector>> &GetEntries(const Vector &vector);
	static void AddEntry(Vector &vector, string name, unique_ptr<Vector> entry);
};

struct SequenceVector {
	static void GetSequence(const Vector &vector, int64_t &start, int64_t &increment) {
		assert(vector.vector_type == VectorType::SEQUENCE_VECTOR);
		auto data = (int64_t *)vector.buffer->GetData();
		start = data[0];
		increment = data[1];
	}
};

class StandaloneVector : public Vector {
public:
	StandaloneVector() : Vector() {
	}
	StandaloneVector(TypeId type) : Vector(type) {
	}
	StandaloneVector(TypeId type, data_ptr_t dataptr) : Vector(type, dataptr) {
	}

public:
	idx_t size() {
		return count;
	}
	void SetCount(idx_t count) {
		assert(count <= STANDARD_VECTOR_SIZE);
		this->count = count;
	}

protected:
	idx_t count;
};

} // namespace duckdb
