#include "include/pubmed.hpp"

#include "duckdb/common/file_system.hpp"

#include <iostream>
#include <fstream>

using namespace duckdb;
using namespace std;

namespace pubmed {

void dbgen_small(duckdb::Connection &conn, std::string rai_stmt) {
	conn.Query("BEGIN TRANSACTION");
	auto result = conn.Query("CREATE TABLE documents (doc_id BIGINT, year INTEGER);");
	// cout << result->ToString() << endl;
	result = conn.Query("CREATE TABLE authors (au_id BIGINT, name VARCHAR);");
	// cout << result->ToString() << endl;
	result = conn.Query("CREATE TABLE terms (t_id BIGINT, term VARCHAR);");
	// cout << result->ToString() << endl;
	result = conn.Query("CREATE TABLE da (doc_id BIGINT, au_id BIGINT);");
	// cout << result->ToString() << endl;
	result = conn.Query("CREATE TABLE dt (doc_id BIGINT, t_id BIGINT, freq INTEGER);");
	// cout << result->ToString() << endl;
	string data_file_path = "third_party/pubmed/data/small/";
	string tables[] = {"documents", "authors", "terms", "da", "dt"};
	for (auto &table : tables) {
		string data_file_name = data_file_path + table + ".csv";
		auto file_system = make_unique<FileSystem>();
		if (!file_system->FileExists(data_file_name)) {
			throw Exception("PubMed data file " + data_file_name + " missing.");
		}
		result = conn.Query("COPY " + table + " FROM '" + data_file_name + "' DELIMITER ',' ESCAPE '\\';");
		cout << result->ToString() << endl;
	}
	conn.Query("COMMIT");
	cout << "---> Creating RAIS" << endl;
	result = conn.Query(rai_stmt);
	result->Print();
}

void dbgen_large(duckdb::Connection &conn, std::string rai_stmt) {
	conn.Query("BEGIN TRANSACTION");
	auto result = conn.Query("CREATE TABLE documents (doc_id BIGINT, year INTEGER);");
	// cout << result->ToString() << endl;
	result = conn.Query("CREATE TABLE authors (au_id BIGINT, name VARCHAR);");
	// cout << result->ToString() << endl;
	result = conn.Query("CREATE TABLE terms (t_id BIGINT, term VARCHAR);");
	// cout << result->ToString() << endl;
	result = conn.Query("CREATE TABLE da (doc_id BIGINT, au_id BIGINT);");
	// cout << result->ToString() << endl;
	result = conn.Query("CREATE TABLE dt (doc_id BIGINT, t_id BIGINT, fre INTEGER);");
	// cout << result->ToString() << endl;
	string data_file_path = "third_party/pubmed/data/large/";
	string tables[] = {"documents", "authors", "terms", "da", "dt"};
	for (auto &table : tables) {
		string data_file_name = data_file_path + table + ".csv";
		auto file_system = make_unique<FileSystem>();
		if (!file_system->FileExists(data_file_name)) {
			throw Exception("PubMed data file " + data_file_name + " missing.");
		}
		result = conn.Query("COPY " + table + " FROM '" + data_file_name + "' DELIMITER ',' ESCAPE '\\';");
		cout << result->ToString() << endl;
	}
	conn.Query("COMMIT");
	cout << "---> Creating RAIS" << endl;
	result = conn.Query(rai_stmt);
	result->Print();
}

string get_jo(string name) {
	string jo_file_name = "third_party/pubmed/jos/" + name + ".json";
	// string jo_file_name = "/Users/guodong/Developer/graindb/third_party/pubmed/jos/" + jo_name + ".json";
	auto file_system = make_unique<FileSystem>();
	if (!file_system->FileExists(jo_file_name)) {
		cout << "Not found jos" << endl;
		return "";
	}
	ifstream ifs(jo_file_name);
	string jo_json((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
	return jo_json;
}

/*
 * dt1 -- t -- dt2
 * SELECT dt2.doc_id, COUNT(*)
FROM dt dt1 JOIN (terms t JOIN dt dt2 ON t.t_id=dt2.t_id) ON dt1.t_id=t.t_id
WHERE dt1.doc_id=:tid GROUP BY dt2.doc_id
 * */
string get_sd(int64_t doc_id) {
	string q = "SELECT COUNT(*) "
	           "FROM dt dt1 JOIN (terms t JOIN dt dt2 ON t.t_id=dt2.t_id) ON dt1.t_id=t.t_id "
	           "WHERE dt1.doc_id=" + to_string(doc_id);
	           //" GROUP BY dt2.doc_id";
	// cout << q << endl;
	return q;
}

/*
 * d1 -- dt1 -- t -- dt2 -- d2
 * SELECT dt2.doc_id, COUNT(*)
FROM (((documents d1 JOIN dt dt1 ON d1.doc_id=dt1.doc_id) JOIN terms t ON dt1.t_id=t.t_id) JOIN dt dt2 ON t.t_id=dt2.t_id) JOIN documents d2 ON dt2.doc_id=d2.doc_id
WHERE d1.doc_id=:id GROUP BY dt2.doc_id;
 * */
string get_fsd(int64_t doc_id) {
	string q =
	    "SELECT COUNT(*) "
	    "FROM (((documents d1 JOIN dt dt1 ON d1.doc_id=dt1.doc_id) JOIN terms t ON dt1.t_id=t.t_id) JOIN dt dt2 ON t.t_id=dt2.t_id) JOIN documents d2 ON dt2.doc_id=d2.doc_id"
	    " WHERE d1.doc_id=" + to_string(doc_id);
	    //" GROUP BY dt2.doc_id";
	// cout << q << endl;
	return q;
}

/*
 * [a1] -- da1 -- d1 -- dt1 -- t -- dt2 -- d2 -- da2 -- [a2]
 * SELECT da2.au_id, COUNT(*)
FROM ((((((da da1 JOIN documents d1 ON da.doc_id=d1.doc_id) JOIN dt dt1 ON d.doc_id=dt1.doc_id) JOIN terms t ON dt1.t_id=t.t_id) JOIN dt dt2 ON t.t_id=dt2.t_id) JOIN documents d2 ON dt2.doc_id=d2.doc_id) JOIN da da2 ON d2.doc_id=da2.doc_id)
WHERE da1.au_id=:id GROUP BY da2.au_id;
 * */
string get_as(int64_t author_id) {
	string q = "SELECT COUNT(*) "
	           "FROM (((((da da1 JOIN documents d1 ON da1.doc_id=d1.doc_id) JOIN dt dt1 ON d1.doc_id=dt1.doc_id) JOIN terms t ON dt1.t_id=t.t_id) JOIN dt dt2 ON t.t_id=dt2.t_id) JOIN documents d2 ON dt2.doc_id=d2.doc_id) JOIN da da2 ON d2.doc_id=da2.doc_id"
	           " WHERE da1.au_id=" + to_string(author_id);
	           //" GROUP BY da2.au_id";
	// cout << q << endl;
	return q;
}

/*
 * SELECT da.au_id, COUNT(*)
FROM da WHERE da.doc_id IN
((SELECT dt.doc_id FROM dt WHERE dt.t_id=:t1id)
 INTERSECT (SELECT d.doc_id FROM documents d WHERE d.year>2012)
 INTERSECT ((SELECT da.doc_id FROM da JOIN documents d ON da.doc_id=d.doc_id) JOIN dt ON da.doc_id=dt.doc_id WHERE dt.t_id=:t2id)
) GROUP BY da.au_id;";
 * */
std::string get_ad(int64_t term1_id, int64_t term2id) {
	string q =
	    "SELECT COUNT(*) FROM da WHERE da.doc_id IN ((SELECT dt.doc_id FROM dt WHERE dt.t_id=" +
	    to_string(term1_id) +
	    ") INTERSECT (SELECT d.doc_id FROM documents d WHERE d.year>2012) INTERSECT (SELECT da.doc_id FROM (da JOIN documents d ON da.doc_id=d.doc_id) JOIN "
	    "dt ON da.doc_id=dt.doc_id WHERE dt.t_id=" +
	    to_string(term2id) + "))";
	//" GROUP BY da.au_id;";
	// cout << q << endl;
	return q;
}

std::string get_query(int id, int64_t param1, int64_t param2) {
	switch (id) {
	case 1:
		return get_sd(param1);
	case 2:
		return get_fsd(param1);
	case 3:
		return get_as(param1);
	case 4:
		return get_ad(param1, param2);
	default:
		return "";
	}
}
} // namespace pubmed
