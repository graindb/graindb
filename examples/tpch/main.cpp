#include "duckdb.hpp"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>
#include <stdlib.h>
#include <string>

using namespace std;
using namespace duckdb;

#define QUERY_REPETITION 4
#define DATA_DIR "/data/data_100g/"
#define QUERY_DIR "/data/gen_queries/queries100/"

// trim from start (in place)
static inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}

int main() {
	DuckDB db(nullptr);
	Connection con(db);

	string data_dir = DATA_DIR;
	string query_dir = QUERY_DIR;

	if (data_dir.compare(data_dir.back(), 1, "/") == 0) {
		data_dir += "/";
	}
	if (query_dir.compare(query_dir.back(), 1, "/") == 0) {
		query_dir += "/";
	}

	string tables[8] = {"supplier", "customer", "lineitem", "orders", "nation", "region", "part", "partsupp"};

	// drop tables
	for (auto &table : tables) {
		con.Query("DROP TABLE " + table + ";");
	}

	// create tables
	con.Query("CREATE TABLE NATION  ( n_nationkey  INTEGER NOT NULL,n_name CHAR(25) "
	          "NOT NULL,n_regionkey  INTEGER NOT NULL, n_comment   VARCHAR(152));");
	con.Query("CREATE TABLE REGION  ( r_regionkey  INTEGER NOT NULL,  r_name     "
	          "  CHAR(25) NOT NULL, r_comment    VARCHAR(152));");
	con.Query("CREATE TABLE PART  ( p_partkey  INTEGER NOT NULL, p_name  "
	          "VARCHAR(55) NOT NULL,  p_mfgr        CHAR(25) NOT NULL,    p_brand "
	          "CHAR(10) NOT NULL, p_type        VARCHAR(25) NOT NULL, p_size        "
	          "INTEGER NOT NULL, p_container   CHAR(10) NOT NULL,  p_retailprice "
	          "DECIMAL(15,2) NOT NULL,   p_comment     VARCHAR(23) NOT NULL );");
	con.Query("CREATE TABLE SUPPLIER ( s_suppkey     INTEGER NOT NULL, s_name     "
	          "CHAR(25) NOT NULL, s_address     VARCHAR(40) NOT NULL,  s_nationkey   "
	          "INTEGER NOT NULL, s_phone       CHAR(15) NOT NULL,  s_acctbal     "
	          "DECIMAL(15,2) NOT NULL, s_comment     VARCHAR(101) NOT NULL);");
	con.Query("CREATE TABLE PARTSUPP ( ps_partkey     INTEGER NOT NULL, ps_suppkey     "
	          "INTEGER NOT NULL, ps_availqty    INTEGER NOT NULL, ps_supplycost  "
	          "DECIMAL(15,2)  NOT NULL,ps_comment   VARCHAR(199) NOT NULL );");
	con.Query("CREATE TABLE CUSTOMER ( c_custkey     INTEGER NOT NULL,  c_name "
	          "     VARCHAR(25) NOT NULL, c_address     VARCHAR(40) NOT NULL, "
	          "c_nationkey   INTEGER NOT NULL, c_phone   CHAR(15) NOT NULL, "
	          "c_acctbal     DECIMAL(15,2)   NOT NULL, c_mktsegment  CHAR(10) "
	          "NOT NULL, c_comment  VARCHAR(117) NOT NULL);");
	con.Query("CREATE TABLE ORDERS  ( o_orderkey  INTEGER NOT "
	          "NULL,o_custkey  INTEGER NOT NULL,   o_orderstatus    "
	          "CHAR(1) NOT NULL, o_totalprice     DECIMAL(15,2) NOT NULL, "
	          "o_orderdate    DATE NOT NULL, o_orderpriority  CHAR(15) NOT "
	          "NULL,o_clerk  CHAR(15) NOT NULL, o_shippriority   INTEGER "
	          "NOT NULL, o_comment  VARCHAR(79) NOT NULL);");
	con.Query("CREATE TABLE LINEITEM ( l_orderkey    INTEGER NOT NULL, l_partkey     "
	          "INTEGER NOT NULL, l_suppkey     INTEGER NOT NULL, l_linenumber  INTEGER "
	          "NOT NULL, l_quantity    DECIMAL(15,2) NOT NULL, l_extendedprice  "
	          "DECIMAL(15,2) NOT NULL, l_discount    DECIMAL(15,2) NOT NULL, l_tax     "
	          "    DECIMAL(15,2) NOT NULL, l_returnflag  CHAR(1) NOT NULL, "
	          "l_linestatus  CHAR(1) NOT NULL, l_shipdate    DATE NOT NULL, "
	          "l_commitdate  DATE NOT NULL, l_receiptdate DATE NOT NULL, "
	          "l_shipinstruct CHAR(25) NOT NULL, l_shipmode     CHAR(10) NOT NULL, "
	          "l_comment      VARCHAR(44) NOT NULL);");

	// import data to tables
	for (auto &table : tables) {
		auto copy_result = con.Query("COPY " + table + " FROM '" + data_dir + table + ".tbl' (DELIMITER '|');");
		cout << copy_result->ToString() + " records are copied into table " + table << endl;
	}

	// evaluate queries
	cout << "======== Query Evaluation ========" << endl;
	for (int i = 1; i <= 22; i++) {
		string line = "";
		string sql = "";
		string query_path = query_dir + to_string(i) + ".sql";
		ifstream in(query_path.c_str());
		if (in) // file exists
		{
			while (getline(in, line)) // line does not contain \n
			{
				trim(line);
				sql = sql + " " + line;
			}
		} else // file null
		{
			cout << "Query file for Q-" + to_string(i) + " not found." << endl;
			continue;
		}
		auto t1 = chrono::high_resolution_clock::now();
		auto result = con.Query(sql);
		auto t2 = chrono::high_resolution_clock::now();
		if (!result->success) {
			cout << "Q-" << i << " execution failed: " << result->error << endl;
			cout << endl;
			continue;
		}
		auto duration = chrono::duration_cast<chrono::microseconds>(chrono::duration<double>(t2 - t1));
		cout << "Q-" << i << ", " << 0 << ", " << duration.count() << "us" << endl;

		cout << "Q-" << i << ", " << 0 << ", get result :" << result->collection.count << "rows" << endl;
		for (int j = 1; j < QUERY_REPETITION; j++) {
			auto t1 = chrono::high_resolution_clock::now();
			auto result = con.Query(sql);
			auto t2 = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::microseconds>(chrono::duration<double>(t2 - t1));
			cout << "Q-" << i << ", " << j << ", " << duration.count() << "us" << endl;
		}
		cout << endl;
	}
}
