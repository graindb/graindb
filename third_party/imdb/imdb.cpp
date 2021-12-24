#include "include/imdb.hpp"

#include "duckdb/common/file_system.hpp"
#include "imdb_constants.hpp"

#include <fstream>

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
		}
	}
}

string get_113_default_jo(const string& jo_name) {
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

string get_113_optimized_jo(const string& jo_name) {
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

string get_113_query(int query) {
	if (query <= 0 || query > IMDB_113_QUERIES_COUNT) {
		throw SyntaxException("Out of range IMDB query number %d", query);
	}
	return IMDB_113_QUERIES[query - 1];
}

string get_ddl(int tid) {
	if (tid < 0 || tid > IMDB_TABLE_COUNT) {
		throw SyntaxException("Out of range IMDB table number %d", tid);
	}
	return IMDB_TABLE_DDL[tid];
}

} // namespace imdb
