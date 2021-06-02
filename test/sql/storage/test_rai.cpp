#include "catch.hpp"
#include "duckdb/catalog/catalog.hpp"
#include "duckdb/catalog/catalog_entry/schema_catalog_entry.hpp"
#include "duckdb/catalog/catalog_entry/table_catalog_entry.hpp"
#include "duckdb/common/types/chunk_collection.hpp"
#include "duckdb/common/types/data_chunk.hpp"
#include "duckdb/storage/alist.hpp"
#include "duckdb/storage/data_table.hpp"
#include "duckdb/storage/rai.hpp"
#include "test_helpers.hpp"

using namespace duckdb;
using namespace std;

TEST_CASE("Test rai append", "[rai]") {
	unique_ptr<QueryResult> result;
	DuckDB db(nullptr);
	Connection con(db);

	con.Query("CREATE TABLE a(id INTEGER)");
	con.Query("CREATE TABLE b(a_id INTEGER, c_id INTEGER)");
	con.Query("CREATE TABLE c(id INTEGER)");

	//! Insert values into a
	for (idx_t i = 0; i < 10; i++) {
		auto result = con.Query("INSERT INTO a VALUES (" + to_string(i) + ")");
		REQUIRE(CHECK_COLUMN(result, 0, {1}));
	}
	//! Insert values into c
	for (idx_t i = 0; i < 1500; i++) {
		auto result = con.Query("INSERT INTO c VALUES (" + to_string(i) + ")");
		REQUIRE(CHECK_COLUMN(result, 0, {1}));
	}
	//! Insert values into b
	for (idx_t i = 0; i < 10; i++) {
		auto inserted_tuples = (i * 150) + 1;
		for (idx_t j = 0; j < inserted_tuples; j++) {
			auto result = con.Query("INSERT INTO b VALUES (" + to_string(i) + ", " + to_string(j) + ")");
			REQUIRE(CHECK_COLUMN(result, 0, {1}));
		}
	}
	//! Create RAI on b
	con.Query("CREATE RAI b_rai ON b (FROM a_id REFERENCES a.id, TO c_id REFERENCES c.id)");
	//! Get AList of b_rai
	con.Query("BEGIN TRANSACTION");
	auto &catalog = Catalog::GetCatalog(*con.context);
	auto schema = catalog.GetSchema(*con.context);
	auto table = reinterpret_cast<TableCatalogEntry *>(schema->GetEntry(*con.context, CatalogType::TABLE, "b", true));
	auto &alist = table->storage->info->rais[0]->alist->compact_forward_list;
	//! Check result
	idx_t edge_id = 0;
	for (idx_t i = 0; i < 10; i++) {
		auto expected_count = (i * 150) + 1;
		auto offset = alist.offsets[i];
		REQUIRE(alist.sizes[i] == expected_count);
		for (idx_t j = 0; j < expected_count; j++) {
			REQUIRE(alist.edges[offset + j] == edge_id++);
			REQUIRE(alist.vertices[offset + j] == j);
		}
	}
	con.Query("COMMIT");
}

TEST_CASE("Test rai fetch", "[rai]") {
	unique_ptr<QueryResult> result;
	DuckDB db(nullptr);
	Connection con(db);

	con.Query("CREATE TABLE a(id INTEGER)");
	con.Query("CREATE TABLE b(a_id INTEGER, c_id INTEGER)");
	con.Query("CREATE TABLE c(id INTEGER)");

	const idx_t S_NUM = 15;
	const idx_t V_NUM = 1500;
	const idx_t E_NUM = 100;
	//! Insert values into a
	for (idx_t i = 0; i < S_NUM; i++) {
		result = con.Query("INSERT INTO a VALUES (" + to_string(i) + ")");
		REQUIRE(CHECK_COLUMN(result, 0, {1}));
	}
	//! Insert values into c
	for (idx_t i = 0; i < V_NUM; i++) {
		result = con.Query("INSERT INTO c VALUES (" + to_string(i) + ")");
		REQUIRE(CHECK_COLUMN(result, 0, {1}));
	}
	//! Insert values into b
	for (idx_t i = 0; i < S_NUM; i++) {
		auto inserted_tuples = (i * E_NUM) + 1;
		string sql = "INSERT INTO b VALUES ";
		for (idx_t j = 0; j < inserted_tuples; j++) {
			sql += "(" + to_string(i) + ", " + to_string(j) + "), ";
		}
		sql = sql.substr(0, sql.size() - 2);
		sql += ";";
		result = con.Query(sql);
		vector<Value> values = {Value::BIGINT(inserted_tuples)};
		REQUIRE(CHECK_COLUMN(result, 0, values));
	}
	//! Create RAI on b
	con.Query("CREATE RAI b_rai ON b (FROM a_id REFERENCES a.id, TO c_id REFERENCES c.id)");
	//! Get AList of b_rai
	con.Query("BEGIN TRANSACTION");
	auto &catalog = Catalog::GetCatalog(*con.context);
	auto schema = catalog.GetSchema(*con.context);
	auto table = reinterpret_cast<TableCatalogEntry *>(schema->GetEntry(*con.context, CatalogType::TABLE, "b", true));
	auto &rai = table->storage->info->rais[0];
	//! Check result
	vector<TypeId> result_types = {TypeId::INT64, TypeId::VARCHAR, TypeId::INT64, TypeId::INT64};
	vector<TypeId> rid_types = {TypeId::INT64};
	vector<TypeId> right_types = {TypeId::INT64, TypeId::VARCHAR};
	ChunkCollection rid_chunks;
	DataChunk rid_chunk;
	rid_chunk.Initialize(rid_types);
	rid_chunk.SetCardinality(S_NUM);
	ChunkCollection right_chunks;
	DataChunk right_chunk;
	right_chunk.Initialize(right_types);
	right_chunk.SetCardinality(S_NUM);
	for (idx_t i = 0; i < S_NUM; i++) {
		rid_chunk.SetValue(0, i, Value::BIGINT(i));
		right_chunk.SetValue(0, i, Value::BIGINT(10244));
		right_chunk.SetValue(1, i, Value::CreateValue("102441024410244"));
	}
	rid_chunks.Append(rid_chunk);
	right_chunks.Append(right_chunk);
	rid_chunks.count = S_NUM;
	right_chunks.count = S_NUM;

	ChunkCollection chunks;
	DataChunk new_chunk;
	new_chunk.Initialize(result_types);
	idx_t left_tuple = 0, right_tuple = 0, right_chunk_idx = 0;
	do {
		if (right_tuple >= rid_chunks.chunks[right_chunk_idx]->size()) {
			right_chunk_idx++;
			if (right_chunk_idx >= rid_chunks.chunks.size()) {
				break;
			}
		}
		new_chunk.Reset();
		auto &rid_chunk = *rid_chunks.chunks[right_chunk_idx];
		auto &right_chunk = *right_chunks.chunks[right_chunk_idx];
		rai->GetChunk(right_chunk, rid_chunk, new_chunk, left_tuple, right_tuple, true);
		chunks.Append(new_chunk);
	} while (new_chunk.size() == 0 || right_chunk_idx < rid_chunks.chunks.size());

	idx_t row_id = 0;
	auto col1 = Value::BIGINT(10244);
	auto col2 = Value::CreateValue("102441024410244");
	for (idx_t i = 0; i < S_NUM; i++) {
		auto count = (i * E_NUM) + 1;
		for (idx_t j = 0; j < count; j++) {
			auto row = chunks.GetRow(row_id);
			REQUIRE(row[0] == col1);
			REQUIRE(row[1] == col2);
			REQUIRE(row[2] == Value::BIGINT(row_id++));
			REQUIRE(row[3] == Value::BIGINT(j));
		}
	}
	REQUIRE(chunks.count == row_id);
	con.Query("COMMIT");
}
