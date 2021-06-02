//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/common/constants.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <cmath>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

namespace duckdb {

//! inline std directives that we use frequently
using std::move;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using data_ptr = unique_ptr<char[]>;
using std::make_shared;
using std::vector;

// NOTE: there is a copy of this in the Postgres' parser grammar (gram.y)
#define DEFAULT_SCHEMA "main"
#define TEMP_SCHEMA "temp"
#define INVALID_SCHEMA ""

//! The vector size used in the execution engine
#ifndef STANDARD_VECTOR_SIZE
#define STANDARD_VECTOR_SIZE 1024
#endif

#if ((STANDARD_VECTOR_SIZE & (STANDARD_VECTOR_SIZE - 1)) != 0)
#error Vector size should be a power of two
#endif

//! Prams for enabling join order injection and cardinality injection
#define ENABLE_CARD_INJECTION false
#define ENABLE_JOIN_ORDER_INJECTION true

#define ENABLE_PROFILING false

//! Params for experiment evaluation
#define ENABLE_ALISTS true
#define ENABLE_RAI_JOIN_MERGE true
#define ENABLE_RID_HT false
#define ENABLE_LOOKUP_JOIN false
#define ENABLE_LOOKUP_FILTER false
#define ENABLE_ZONE_FILTER false
#define ENABLE_ADAPTIVE_FILTER false
#define ENABLE_ADAPTIVE_JOIN true

//! a saner size_t for loop indices etc
typedef uint64_t idx_t;

//! The type used for row identifiers
typedef int64_t row_t;

//! The type used for hashes
typedef uint64_t hash_t;

//! The value used to signify an invalid index entry
extern const idx_t INVALID_INDEX;

//! data pointers
typedef uint8_t data_t;
typedef data_t *data_ptr_t;
typedef const data_t *const_data_ptr_t;

//! Type used to represent dates
typedef int32_t date_t;
//! Type used to represent time
typedef int32_t dtime_t;
//! Type used to represent timestamps
typedef int64_t timestamp_t;
//! Type used for the selection vector
typedef uint16_t sel_t;
//! Type used for transaction timestamps
typedef idx_t transaction_t;

//! Type used for column identifiers
typedef idx_t column_t;
//! Special value used to signify the ROW ID of a table
extern const column_t COLUMN_IDENTIFIER_ROW_ID;

//! The maximum row identifier used in tables
extern const row_t MAX_ROW_ID;

//! Zero selection vector: completely filled with the value 0 [READ ONLY]
extern const sel_t ZERO_VECTOR[STANDARD_VECTOR_SIZE];

extern const transaction_t TRANSACTION_ID_START;
extern const transaction_t MAXIMUM_QUERY_ID;
extern const transaction_t NOT_DELETED_ID;

extern const double PI;

struct Storage {
	//! The size of a hard disk sector, only really needed for Direct IO
	constexpr static int SECTOR_SIZE = 4096;
	//! Block header size for blocks written to the storage
	constexpr static int BLOCK_HEADER_SIZE = sizeof(uint64_t);
	// Size of a memory slot managed by the StorageManager. This is the quantum of allocation for Blocks on DuckDB. We
	// default to 256KB. (1 << 18)
	constexpr static int BLOCK_ALLOC_SIZE = 262144;
	//! The actual memory space that is available within the blocks
	constexpr static int BLOCK_SIZE = BLOCK_ALLOC_SIZE - BLOCK_HEADER_SIZE;
	//! The size of the headers. This should be small and written more or less atomically by the hard disk. We default
	//! to the page size, which is 4KB. (1 << 12)
	constexpr static int FILE_HEADER_SIZE = 4096;
};

struct CostModel {
	constexpr static double ALPHA = 0.8;
	constexpr static double BETA = 1.2;
};

struct SIPJoin {
	constexpr static idx_t NLAJ_MAGIC = 2048;
	constexpr static double SHJ_MAGIC = 0.8;
};

struct RAIJoin {
	constexpr static idx_t MAX_ROWS_SIZE = 2048;
	constexpr static double MAX_ZONE_RATE = 0.8;
};

uint64_t NextPowerOfTwo(uint64_t v);

} // namespace duckdb
