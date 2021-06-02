#include "include/imdb.hpp"

#include "duckdb/common/file_system.hpp"
#include "imdb_constants.hpp"

#include <chrono>
#include <fstream>
#include <iostream>

using namespace duckdb;
using namespace std;

namespace imdb {

void dbgen(Connection &conn, bool enableRAIs, bool enableLoadData) {
	conn.Query("BEGIN TRANSACTION");
	for (int t = 0; t < IMDB_TABLE_COUNT; t++) {
		conn.Query(IMDB_TABLE_DDL[t]);
		if (!enableLoadData) {
			continue;
		}
		string table_name = string(IMDB_TABLE_NAMES[t]);
		string data_file_name = "third_party/imdb/data/" + table_name + ".csv.gz";
		auto file_system = make_unique<FileSystem>();
		if (!file_system->FileExists(data_file_name)) {
			throw Exception("IMDB data file " + data_file_name + " missing, try `make imdb` to download.");
		}
		conn.Query("COPY " + table_name + " FROM '" + data_file_name + "' DELIMITER ',' ESCAPE '\\';");
	}
	conn.Query("COMMIT");
	if (enableRAIs) {
		for (auto &rai : IMDB_RAIS) {
			auto result = conn.Query(rai);
			//			result->Print();
		}
	}
}

void dbgen_small(Connection &conn, bool enableRAIs, bool enableLoadData) {
	conn.Query("BEGIN TRANSACTION");
	for (int t = 0; t < IMDB_TABLE_COUNT; t++) {
		conn.Query(IMDB_TABLE_DDL[t]);
		if (!enableLoadData) {
			continue;
		}
		string table_name = string(IMDB_TABLE_NAMES[t]);
		string data_file_name = "/Users/guodong/Developer/graindb/third_party/imdb/small_data/" + table_name + ".csv";
		auto file_system = make_unique<FileSystem>();
		if (!file_system->FileExists(data_file_name)) {
			throw Exception("IMDB small data file " + data_file_name + " missing.");
		}
		conn.Query("COPY " + table_name + " FROM '" + data_file_name + "' DELIMITER ',' ESCAPE '\\';");
	}
	conn.Query("COMMIT");
	if (enableRAIs) {
		for (auto &rai : IMDB_RAIS) {
			auto result = conn.Query(rai);
			//			result->Print();
		}
	}
}

string get_113_default_jo(string jo_name) {
	if (jo_name == "EMPTY") {
		return "{}";
	}
	string jo_file_name = "third_party/imdb/jos_113/default/" + jo_name + ".json";
	auto file_system = make_unique<FileSystem>();
	if (!file_system->FileExists(jo_file_name)) {
		return "";
	}
	ifstream ifs(jo_file_name);
	string jo_json((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
	return jo_json;
}

string get_113_optimized_jo(string jo_name) {
	if (jo_name == "EMPTY") {
		return "{}";
	}
	string jo_file_name = "third_party/imdb/jos_113/optimized/" + jo_name + ".json";
	auto file_system = make_unique<FileSystem>();
	if (!file_system->FileExists(jo_file_name)) {
		return "";
	}
	ifstream ifs(jo_file_name);
	string jo_json((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
	return jo_json;
}

string get_113_card(string card_name) {
	if (card_name == "EMPTY") {
		return "";
	}
	string card_file_name = "third_party/imdb/card_113/" + card_name + ".json";
	//	string card_file_name = "/Users/guodong/Developer/graindb/third_party/imdb/card_113/" + card_name + ".json";
	auto file_system = make_unique<FileSystem>();
	if (!file_system->FileExists(card_file_name)) {
		return "";
	}
	ifstream ifs(card_file_name);
	string card_json((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
	return card_json;
}

string get_113_query(int query) {
	if (query <= 0 || query > IMDB_113_QUERIES_COUNT) {
		throw SyntaxException("Out of range IMDB query number %d", query);
	}
	return IMDB_113_QUERIES[query - 1];
}

int get_query_group_id(int query) {
	if (query <= 0 || query > IMDB_113_QUERIES_COUNT) {
		throw SyntaxException("Out of range IMDB query number %d", query);
	}
	return IMDB_QUERY_GROUP[query - 1];
}

string get_table(int tid) {
	if (tid < 0 || tid > IMDB_TABLE_COUNT) {
		throw SyntaxException("Out of range IMDB table number %d", tid);
	}
	return IMDB_TABLE_NAMES[tid];
}

string get_ddl(int tid) {
	if (tid < 0 || tid > IMDB_TABLE_COUNT) {
		throw SyntaxException("Out of range IMDB table number %d", tid);
	}
	return IMDB_TABLE_DDL[tid];
}

string get_answer(int query) {
	if (query <= 0 || query > IMDB_113_QUERIES_COUNT) {
		throw SyntaxException("Out of range IMDB query number %d", query);
	}
	return IMDB_ANSWERS[query - 1];
}

} // namespace imdb
