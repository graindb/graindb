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
		// result->Print();
	}
	conn.Query("COMMIT");
	if (enableRAIs) {
		for (auto &rai_stmt : LDBC_RAIS) {
			auto result = conn.Query(rai_stmt);
			// result->Print();
		}
	}
	for (auto &i : LDBC_INDEX) {
		auto result = conn.Query(i);
		// result->Print();
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
		if (qgid == 1) {
			if (table_name != "person" && table_name != "comment") {
				continue;
			}
		} else if (qgid == 2 || qgid == 5) {
			if (table_name != "forum" && table_name != "post") {
				continue;
			}
		} else if (qgid == 3) {
			if (table_name != "person" && table_name != "forum_person" && table_name != "forum") {
				continue;
			}
		} else if (qgid == 4) {
			if (table_name != "person" && table_name != "likes_comment" && table_name != "comment") {
				continue;
			}
		} else if (qgid == 6 || qgid == 7) {
			if (table_name != "person" && table_name != "likes_comment") {
				continue;
			}
		} else if (qgid == 8 || qgid == 9) {
			if (table_name != "person" && table_name != "knows") {
				continue;
			}
		}
		string data_file_name = "third_party/ldbc/data/sf" + to_string(sf) + "/" + table_name + ".csv";
		auto file_system = make_unique<FileSystem>();
		if (!file_system->FileExists(data_file_name)) {
			throw Exception("LDBC data file " + data_file_name + " missing.");
		}
		result = conn.Query("COPY " + table_name + " FROM '" + data_file_name +
		                    "' WITH ( DELIMITER ',', HEADER 1, ESCAPE '\\');");
		// result->Print();
	}
	conn.Query("COMMIT");
	if (enableRAIs) {
		if (qgid == 1) {
			auto result = conn.Query(LDBC_RAIS[1]);
			result->Print();
		} else if (qgid == 2 || qgid == 5) {
			auto result = conn.Query(LDBC_RAIS[8]);
			result->Print();
		} else if (qgid == 3) {
			auto result = conn.Query(LDBC_RAIS[10]);
			result->Print();
		} else if (qgid == 4) {
			auto result = conn.Query(LDBC_RAIS[14]);
			result->Print();
		} else if (qgid == 6 || qgid == 7) {
			auto result =
			    conn.Query("CREATE PKFK RAI per_lcmt ON likes_comment (FROM l_personid REFERENCES person.p_personid, "
			               "TO l_personid REFERENCES person.p_personid);");
			result->Print();
		} else if (qgid == 8 || qgid == 9) {
			auto result = conn.Query("CREATE PKFK RAI knows_r ON knows (FROM k_person1id REFERENCES person.p_personid, "
			                         "TO k_person1id REFERENCES person.p_personid);");
			result->Print();
		}
	}
	//	for (auto &i : LDBC_INDEX) {
	//		auto result = conn.Query(i);
	// result->Print();
	//	}
}

std::string ldbc::get_query(int query, int sf) {
	if (query <= 0 || query > LDBC_QUERIES_COUNT) {
		throw SyntaxException("Out of range LCBD SNB query number %d", query);
	}
	switch (sf) {
	case 1:
		return LDBC_QUERIES_SF1[query - 1];
	case 10:
		return LDBC_QUERIES_SF10[query - 1];
	case 30:
		return LDBC_ANSWERS_SF30[query - 1];
	default:
		return "";
	}
}

std::string ldbc::get_light_query(int query, int sf) {
	if (query <= 0 || query > LDBC_LIGHT_QUERIES_COUNT) {
		throw SyntaxException("Out of range LCBD SNB query number %d", query);
	}
	if (sf <= 1) {
		return LDBC_LIGHT_QUERIES_SF1[query - 1];
	} else if (sf == 10) {
		return LDBC_LIGHT_QUERIES_SF10[query - 1];
	} else if (sf == 30) {
		return LDBC_LIGHT_QUERIES_SF30[query - 1];
	} else {
		throw SyntaxException("Not supported LCBD SNB scale factor %d for queries", sf);
	}
}

std::string ldbc::get_micro_query(int query, int param) {
	if (query <= 0 || query > LDBC_MICRO_QUERIES_COUNT) {
		throw SyntaxException("Out of range LCBD SNB query number %d", query);
	}
	return LDBC_QUERIES_MICRO[query - 1] + to_string(param) + ";";
}

std::string ldbc::get_default_jo(std::string jo_name) {
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

std::string ldbc::get_optimized_jo(std::string jo_name) {
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
