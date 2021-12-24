#include "include/ldbc.hpp"

#include "ldbc_constants.hpp"

#include <fstream>
#include <iostream>

using namespace std;
using namespace duckdb;

string ldbc::get_table_name(int num) {
	if (num < 0 || num >= LDBC_TABLE_COUNT) {
		throw SyntaxException("Out of range LCBD SNB table number %d", num);
	}
	return LDBC_TABLE_NAMES[num];
}

string ldbc::get_ddl(int num) {
	if (num < 0 || num >= LDBC_TABLE_COUNT) {
		throw SyntaxException("Out of range LCBD SNB table number %d", num);
	}
	return LDBC_TABLE_DDL[num];
}

void ldbc::dbgen(duckdb::Connection &conn, int sf, bool enableRAIs, bool enableLoading) {
	if (enableLoading && sf == 0) {
		enableLoading = false;
	}
	conn.Query("BEGIN TRANSACTION");
	for (int t = 0; t < LDBC_TABLE_COUNT; t++) {
		auto result = conn.Query(LDBC_TABLE_DDL[t]);
		if (!enableLoading) {
			continue;
		}
		string table_name = string(LDBC_TABLE_NAMES[t]);
		string data_file_name = "third_party/ldbc/data/sf" + to_string(sf) + "/" + table_name + ".csv";
		auto file_system = make_unique<FileSystem>();
		if (!file_system->FileExists(data_file_name)) {
			throw Exception("LDBC data file " + data_file_name + " missing.");
		}
		result = conn.Query("COPY " + table_name + " FROM '" + data_file_name +
		                    "' WITH ( DELIMITER ',', HEADER 1, ESCAPE '\\');");
	}
	conn.Query("COMMIT");
	if (enableRAIs) {
		for (auto &rai_stmt : LDBC_RAIS) {
			auto result = conn.Query(rai_stmt);
		}
	}
	for (auto &i : LDBC_INDEX) {
		auto result = conn.Query(i);
	}
}

void ldbc::dbgen_micro(duckdb::Connection &conn, int sf, int qgid, bool enableRAIs, bool enableLoading) {
	if (enableLoading && sf == 0) {
		enableLoading = false;
	}
	conn.Query("BEGIN TRANSACTION");
	for (int t = 0; t < LDBC_TABLE_COUNT; t++) {
		string table_name = string(LDBC_TABLE_NAMES[t]);
		auto result = conn.Query(LDBC_TABLE_DDL[t]);
		if (!enableLoading) {
			continue;
		}
		if (table_name != "person" && table_name != "knows") {
			continue;
		}
		string data_file_name = "third_party/ldbc/data/sf" + to_string(sf) + "/" + table_name + ".csv";
		auto file_system = make_unique<FileSystem>();
		if (!file_system->FileExists(data_file_name)) {
			throw Exception("LDBC data file " + data_file_name + " missing.");
		}
		result = conn.Query("COPY " + table_name + " FROM '" + data_file_name +
		                    "' WITH ( DELIMITER ',', HEADER 1, ESCAPE '\\');");
	}
	conn.Query("COMMIT");
	if (enableRAIs) {
		auto result = conn.Query("CREATE PKFK RAI knows_r ON knows (FROM k_person1id REFERENCES person.p_personid, "
		                         "TO k_person1id REFERENCES person.p_personid);");
	}
}

std::string ldbc::get_light_query(int query, int sf) {
	if (query <= 0 || query > LDBC_LIGHT_QUERIES_COUNT) {
		throw SyntaxException("Out of range LCBD SNB query number %d", query);
	}
	assert(sf == 10);
	return LDBC_LIGHT_QUERIES_SF10[query - 1];
}

std::string ldbc::get_micro_query(int query, int param) {
	if (query <= 0 || query > LDBC_MICRO_QUERIES_COUNT) {
		throw SyntaxException("Out of range LCBD SNB query number %d", query);
	}
	return LDBC_QUERIES_MICRO[query - 1] + to_string(param) + ";";
}

std::string ldbc::read_file(const std::string& file) {
	auto file_system = make_unique<FileSystem>();
	if (!file_system->FileExists(file)) {
		throw invalid_argument("File " + file + " not found.");
	}
	ifstream ifs(file);
	string jo_json((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
	return jo_json;
}

std::string ldbc::get_light_query_with_views(const string& query, int sf) {
	auto file_path = "third_party/ldbc/queries/light/sf10-views/" + query + ".sql";
	return read_file(file_path);
}

std::string ldbc::get_default_jo(const std::string& jo_name) {
	if (jo_name == "EMPTY") {
		return "{}";
	}
	string jo_file_name = "third_party/ldbc/jos/default/" + jo_name + ".json";
	auto file_system = make_unique<FileSystem>();
	if (!file_system->FileExists(jo_file_name)) {
		return "";
	}
	ifstream ifs(jo_file_name);
	string jo_json((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
	return jo_json;
}

std::string ldbc::get_optimized_jo(const std::string& jo_name) {
	if (jo_name == "EMPTY") {
		return "{}";
	}
	string jo_file_name = "third_party/ldbc/jos/optimized/" + jo_name + ".json";
	auto file_system = make_unique<FileSystem>();
	if (!file_system->FileExists(jo_file_name)) {
		return "";
	}
	ifstream ifs(jo_file_name);
	string jo_json((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
	return jo_json;
}

std::string ldbc::get_views_jo(const std::string& jo_name) {
	auto file_path = "third_party/ldbc/jos/views/" + jo_name + ".json";
	return read_file(file_path);
}

std::string ldbc::get_views(const std::string& query) {
	auto file_path = "third_party/ldbc/queries/light/views/" + query + ".sql";
	return read_file(file_path);
}
