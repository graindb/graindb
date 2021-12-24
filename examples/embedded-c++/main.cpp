#include "dbgen.hpp"
#include "duckdb.hpp"
#include "duckdb/catalog/catalog_entry/schema_catalog_entry.hpp"
#include "duckdb/main/client_context.hpp"
#include "duckdb/planner/planner.hpp"
#include "duckdb/storage/data_table.hpp"
#include "duckdb/transaction/transaction.hpp"
#include "imdb.hpp"
#include "ldbc.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

#define N_RUN 1

using namespace std;
using namespace duckdb;

void DebugLDBC() {
	DuckDB db(nullptr);
	Connection con(db);
	auto result = con.Query("SELECT 1;");
	int table_ids[] = {5, 6, 16};
	for (auto id : table_ids) {
		string ddl = ldbc::get_ddl(id);
		con.Query(ddl);
		string table_name = ldbc::get_table_name(id);
		string data_file_name = "/Users/guodong/Developer/graindb/third_party/ldbc/data/sf1/" + table_name + ".csv";
		//		string data_file_name = "/home/g35jin/ldbc_dataset/sf1/social_network/" + table_name + ".csv";
		auto file_system = make_unique<FileSystem>();
		if (!file_system->FileExists(data_file_name)) {
			throw Exception("LDBC data file " + data_file_name + " missing.");
		}
		//		result = con.Query("COPY " + table_name + " FROM '" + data_file_name + "' WITH ( DELIMITER '|', HEADER
		// 1, ESCAPE '\\');");
		result->Print();
	}
	string q = "SELECT p.rowid, COUNT(*) as num FROM person p, knows k WHERE k.k_person1id=p.p_personid GROUP BY "
	           "p.rowid ORDER BY p.rowid;";
	string q1 = "SELECT COUNT(*) FROM person p1, knows WHERE knows.k_person1id=p1.p_personid AND "
	            "p1.p_firstname='Bingbing' AND p1.p_lastname='Zhang' AND p1.p_gender='female' AND p1.p_placeid=471;";
	string q2 = "SELECT COUNT(*) FROM person p1, knows WHERE knows.k_person1id=p1.p_personid AND p1.p_firstname='Abdul "
	            "Rahman' AND p1.p_lastname='Lesmana' AND p1.p_gender='male' AND p1.p_placeid=651;";
	string q3 = "SELECT COUNT(*) FROM person p1, knows WHERE knows.k_person1id=p1.p_personid AND p1.p_firstname='Jack' "
	            "AND p1.p_lastname='Smith';";
	string q4 = "SELECT COUNT(*) FROM person p1, knows WHERE knows.k_person1id=p1.p_personid AND "
	            "p1.p_firstname='Bingbing' AND p1.p_lastname='Zhang' AND p1.p_gender='female';";
	string q5 = "SELECT COUNT(*) FROM person p1, knows WHERE knows.k_person1id=p1.p_personid AND "
	            "p1.p_firstname='Bingbing' AND p1.p_lastname='Zhang';";
	string q6 =
	    "SELECT COUNT(*) FROM person p1, knows WHERE knows.k_person1id=p1.p_personid AND p1.p_birthday<='1980-06-31';";
	string q7 =
	    "SELECT COUNT(*) FROM person p1, knows WHERE knows.k_person1id=p1.p_personid AND p1.p_birthday<='1980-01-31';";
	string q8 =
	    "SELECT COUNT(*) FROM person p1, knows WHERE knows.k_person1id=p1.p_personid AND p1.p_birthday<='1980-02-31';";
	string q9 =
	    "SELECT COUNT(*) FROM person p1, knows WHERE knows.k_person1id=p1.p_personid AND p1.p_birthday<='1980-03-31';";
	string q10 = "SELECT COUNT(t.t_tagclassid) FROM tag t, message m, message_tag mt WHERE t.t_tagid=mt.mt_tagid AND "
	             "mt.mt_messageid=m.m_messageid AND t.t_tagclassid=0;";
	string queries[] = {q10};
	string rai = "CREATE UNDIRECTED RAI tag_msg ON message_tag (FROM mt_tagid REFERENCES tag.t_tagid, TO mt_messageid "
	             "REFERENCES message.m_messageid);";
	idx_t qid = 1;
	con.EnableExplicitJoinOrder(
	    "{\"type\": \"JOIN\", \"children\": [{\"type\": \"SCAN\", \"table\": \"m\"}, {\"type\": \"JOIN\", "
	    "\"children\": [{\"type\": \"SCAN\", \"table\": \"mt\"}, {\"type\": \"SCAN\", \"table\": \"t\"} ] } ] }");
	for (string q : queries) {
		cout << "====== Q" << qid++ << " ======" << endl;
		result = con.Query(q);
		cout << result->ToString();
	}
	result = con.Query(rai);
	result->Print();
	for (string q : queries) {
		cout << "====== Q" << qid++ << " ======" << endl;
		result = con.Query(q);
		cout << result->ToString();
	}
}

void RunnerWrapper(string query, string rais, string jo, vector<string> table_names, int state) {
	// Load dataset
	DuckDB db(nullptr);
	Connection con(db);
	con.Query("BEGIN TRANSACTION");
	for (int t = 0; t < 21; t++) {
		con.Query(imdb::get_ddl(t));
	}
	for (auto &table_name : table_names) {
		//		string data_file_name = "/Users/guodong/Developer/graindb/third_party/imdb/data/" + table_name +
		//".csv.gz";
		string data_file_name = "third_party/imdb/data/" + table_name + ".csv.gz";
		auto file_system = make_unique<FileSystem>();
		if (!file_system->FileExists(data_file_name)) {
			throw Exception("IMDB data file " + data_file_name + " missing, try `make imdb` to download.");
		}
		auto result = con.Query("COPY " + table_name + " FROM '" + data_file_name + "' (DELIMITER ',', ESCAPE '\\');");
		//		result->Print();
	}
	con.Query("COMMIT");
	auto result = con.Query(rais);
	//	result->Print();
	con.EnableExplicitJoinOrder(jo);
	Profiler profiler;
	if (state >= N_RUN) {
		con.EnableProfiling();
		result = con.Query(query);
	} else {
		profiler.Start();
		result = con.Query(query);
		profiler.End();
	}

	if (state == 0) {
		// first run, print result
		cout << result->ToString() << endl;
		cout << to_string(profiler.Elapsed()) << endl;
	} else if (state >= 1 && state < N_RUN) {
		// run performance
		cout << to_string(profiler.Elapsed()) << endl;
	} else if (state >= N_RUN) {
		// profiling
		auto profile = con.GetProfilingInformation(ProfilerPrintFormat::JSON);
		cout << profile << endl;
		con.DisableProfiling();
	}
}

void SpectrumRunner(string qid, vector<string> tables, string query, string rai, idx_t n_jos, string jo_file_dir) {
	auto file_system = make_unique<FileSystem>();
	// run evaluation
	for (idx_t i = 1; i <= n_jos; i++) {
		string jo_file = jo_file_dir + to_string(i) + ".json";
		if (!file_system->FileExists(jo_file)) {
			cout << "JO file not exists!" << endl;
			continue;
		}
		ifstream ifs(jo_file);
		string jo_json((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
		cout << "----------------------" << endl;
		cout << "|| " << qid << "-" << to_string(i) << " ||" << endl;
		cout << "----------------------" << endl;
		for (idx_t n = 0; n < N_RUN; n++) {
			RunnerWrapper(query, rai, jo_json, tables, n);
			this_thread::sleep_for(chrono::seconds(2));
		}
	}
	// run profiling
	for (idx_t i = 1; i <= n_jos; i++) {
		string jo_file = jo_file_dir + to_string(i) + ".json";
		if (!file_system->FileExists(jo_file)) {
			cout << "JO file not exists!" << endl;
			continue;
		}
		ifstream ifs(jo_file);
		string jo_json((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
		cout << "----------------------" << endl;
		cout << "|| " << qid << "-" << to_string(i) << " ||" << endl;
		cout << "----------------------" << endl;
		RunnerWrapper(query, rai, jo_json, tables, N_RUN);
		this_thread::sleep_for(chrono::seconds(2));
	}
}

void IMDB_NEW_Specturm() {
	string q1 =
	    "SELECT MIN(mc.note) AS production_note, MIN(t.title) AS movie_title, MIN(t.production_year) AS movie_year "
	    "FROM company_type ct JOIN (movie_companies mc JOIN (title t JOIN (movie_info_idx mi_idx JOIN info_type it ON "
	    "it.id = mi_idx.info_type_id) ON t.id = mi_idx.movie_id) ON mc.movie_id = t.id) ON ct.id = mc.company_type_id "
	    "WHERE ct.kind = 'production companies' AND it.info = 'bottom 10 rank' AND mc.note NOT LIKE '%(as "
	    "Metro-Goldwyn-Mayer Pictures)%' AND t.production_year BETWEEN 2005 AND 2010;";
	string q2 =
	    "SELECT MIN(t.title) AS movie_title FROM company_name AS cn JOIN (movie_companies AS mc JOIN (movie_keyword AS "
	    "mk JOIN title AS t ON t.id = mk.movie_id JOIN keyword AS k ON mk.keyword_id = k.id) ON mc.movie_id = t.id) ON "
	    "cn.id = mc.company_id WHERE cn.country_code ='[sm]' AND k.keyword ='character-name-in-title';";
	string q3 = "SELECT MIN(t.title) AS movie_title FROM title AS t JOIN movie_info AS mi ON t.id = mi.movie_id JOIN "
	            "(movie_keyword AS mk JOIN keyword AS k ON k.id = mk.keyword_id) ON t.id = mk.movie_id WHERE k.keyword "
	            "LIKE '%sequel%' AND mi.info IN ('Sweden', 'Norway', 'Germany', 'Denmark', 'Swedish', 'Denish', "
	            "'Norwegian', 'German', 'USA', 'American') AND t.production_year > 1990;";
	string q4 = "SELECT MIN(mi_idx.info) AS rating, MIN(t.title) AS movie_title FROM title AS t JOIN (movie_info_idx "
	            "AS mi_idx JOIN info_type AS it ON it.id = mi_idx.info_type_id) ON t.id = mi_idx.movie_id JOIN "
	            "(movie_keyword AS mk JOIN keyword AS k ON k.id = mk.keyword_id) ON t.id = mk.movie_id WHERE it.info "
	            "='rating' AND k.keyword LIKE '%sequel%' AND mi_idx.info > '9.0' AND t.production_year > 2010;";
	string q5 =
	    "SELECT MIN(t.title) AS american_movie FROM title AS t JOIN (movie_companies AS mc JOIN company_type AS ct ON "
	    "ct.id = mc.company_type_id) ON t.id = mc.movie_id JOIN (movie_info AS mi JOIN info_type AS it ON it.id = "
	    "mi.info_type_id) ON t.id = mi.movie_id WHERE ct.kind = 'production companies' AND mc.note NOT LIKE '%(TV)%' "
	    "AND mc.note LIKE '%(USA)%' AND mi.info IN ('Sweden', 'Norway', 'Germany', 'Denmark', 'Swedish', 'Denish', "
	    "'Norwegian', 'German', 'USA', 'American') AND t.production_year > 1990;";
	string q6 = "SELECT MIN(k.keyword) AS movie_keyword, MIN(n.name) AS actor_name, MIN(t.title) AS marvel_movie FROM "
	            "title AS t JOIN (movie_keyword AS mk JOIN keyword AS k ON k.id = mk.keyword_id) ON t.id = mk.movie_id "
	            "JOIN (cast_info AS ci JOIN name AS n ON n.id = ci.person_id) ON t.id = ci.movie_id WHERE k.keyword = "
	            "'marvel-cinematic-universe' AND n.name LIKE '%Downey%Robert%' AND t.production_year > 2014;";
	vector<string> q1_tables = {"company_type", "movie_companies", "title", "movie_info_idx", "info_type"};
	vector<string> q2_tables = {"company_name", "movie_companies", "title", "movie_keyword", "keyword"};
	vector<string> q3_tables = {"title", "keyword", "movie_keyword", "movie_info"};
	vector<string> q4_tables = {"keyword", "movie_keyword", "title", "movie_info_idx", "info_type"};
	vector<string> q5_tables = {"company_type", "movie_companies", "title", "movie_info", "info_type"};
	vector<string> q6_tables = {"keyword", "movie_keyword", "title", "cast_info", "name"};
	string empty_rai = "";
	string q1_rai = "CREATE RAI it_midx_t ON movie_info_idx (FROM info_type_id REFERENCES info_type.id, TO movie_id "
	                "REFERENCES title.id);";
	string q2_rai =
	    "CREATE RAI cn_mc_t ON movie_companies (FROM company_id REFERENCES company_name.id, TO movie_id REFERENCES "
	    "title.id);"
	    "CREATE RAI k_mk_t ON movie_keyword (FROM keyword_id REFERENCES keyword.id, TO movie_id REFERENCES title.id);";
	string q3_rai =
	    "CREATE RAI k_mk_t ON movie_keyword (FROM keyword_id REFERENCES keyword.id, TO movie_id REFERENCES title.id);";
	string q4_rai =
	    "CREATE RAI k_mk_t ON movie_keyword (FROM keyword_id REFERENCES keyword.id, TO movie_id REFERENCES title.id);"
	    "CREATE RAI it_midx_t ON movie_info_idx (FROM info_type_id REFERENCES info_type.id, TO movie_id REFERENCES "
	    "title.id);";
	string q5_rai = "CREATE RAI it_mi_t ON movie_info (FROM info_type_id REFERENCES info_type.id, TO movie_id "
	                "REFERENCES title.id);";
	string q6_rai =
	    "CREATE RAI k_mk_t ON movie_keyword (FROM keyword_id REFERENCES keyword.id, TO movie_id REFERENCES title.id);"
	    "CREATE RAI t_ci_n ON cast_info (FROM movie_id REFERENCES title.id, TO person_id REFERENCES name.id);";
	string q1_jo_file_dir = "/home/g35jin/NaiveJoinOrderOptimizer/imdb_new/jos/Q01/";
	string q2_jo_file_dir = "/home/g35jin/NaiveJoinOrderOptimizer/imdb_new/jos/Q02/";
	string q3_jo_file_dir = "/home/g35jin/NaiveJoinOrderOptimizer/imdb_new/jos/Q03/";
	string q4_jo_file_dir = "/home/g35jin/NaiveJoinOrderOptimizer/imdb_new/jos/Q04/";
	string q5_jo_file_dir = "/home/g35jin/NaiveJoinOrderOptimizer/imdb_new/jos/Q05/";
	string q6_jo_file_dir = "/home/g35jin/NaiveJoinOrderOptimizer/imdb_new/jos/Q06/";
	SpectrumRunner("IMDB_NEW_Q01", q1_tables, q1, q1_rai, 224, q1_jo_file_dir);
	SpectrumRunner("IMDB_NEW_Q02", q2_tables, q2, q2_rai, 224, q2_jo_file_dir);
	SpectrumRunner("IMDB_NEW_Q03", q3_tables, q3, q3_rai, 40, q3_jo_file_dir);
	SpectrumRunner("IMDB_NEW_Q04", q4_tables, q4, q4_rai, 224, q4_jo_file_dir);
	SpectrumRunner("IMDB_NEW_Q05", q5_tables, q5, q5_rai, 224, q5_jo_file_dir);
	SpectrumRunner("IMDB_NEW_Q06", q6_tables, q6, q6_rai, 224, q6_jo_file_dir);
}

void SIPJoinOrderQ1() {
	// tables
	vector<string> table_names = {"keyword", "movie_keyword", "title"};
	// Q1
	string q1 = "SELECT MIN(k.keyword) AS movie_keyword, MIN(t.title) AS movie_title FROM keyword AS k JOIN "
	            "(movie_keyword AS mk JOIN title AS t ON t.id = mk.movie_id) ON k.id = mk.keyword_id AND "
	            "t.production_year>1990 AND k.keyword='character-name-in-title';";
	// RAI
	//	string rai = "CREATE RAI k_mk_t ON movie_keyword (FROM keyword_id REFERENCES keyword.id, TO movie_id REFERENCES
	// title.id);";
	string rai = "";
	string jo_file_dir = "third_party/join_order_test/jos/Q1/";
	//	string jo_file_dir = "/Users/guodong/Developer/graindb/third_party/join_order_test/jos/";
	const idx_t N_JOS = 1;
	auto file_system = make_unique<FileSystem>();
	// run evaluation
	for (idx_t i = 1; i <= N_JOS; i++) {
		string jo_file = jo_file_dir + to_string(i) + ".json";
		if (!file_system->FileExists(jo_file)) {
			cout << "JO file not exists!" << endl;
			continue;
		}
		ifstream ifs(jo_file);
		string jo_json((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
		cout << "----------------------" << endl;
		cout << "|| "
		     << "Q1-" << to_string(i) << " ||" << endl;
		cout << "----------------------" << endl;
		for (idx_t n = 0; n < N_RUN; n++) {
			RunnerWrapper(q1, rai, jo_json, table_names, n);
			this_thread::sleep_for(chrono::seconds(2));
		}
	}
	// run profiling
	for (idx_t i = 1; i <= N_JOS; i++) {
		string jo_file = jo_file_dir + to_string(i) + ".json";
		if (!file_system->FileExists(jo_file)) {
			cout << "JO file not exists!" << endl;
			continue;
		}
		ifstream ifs(jo_file);
		string jo_json((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
		cout << "----------------------" << endl;
		cout << "|| "
		     << "Q1-" << to_string(i) << " ||" << endl;
		cout << "----------------------" << endl;
		RunnerWrapper(q1, rai, jo_json, table_names, N_RUN + 1);
		this_thread::sleep_for(chrono::seconds(2));
	}
}

void SIPJoinOrderQ2() {
	// tables
	vector<string> table_names = {"keyword", "movie_keyword", "title", "cast_info", "name"};
	// Q2
	string q2 = "SELECT MIN(k.keyword) AS movie_keyword, MIN(t.title) AS marvel_movie, MIN(n.name) AS actor_name FROM "
	            "keyword AS k JOIN (movie_keyword AS mk JOIN (title AS t JOIN (cast_info AS ci JOIN name AS n ON n.id "
	            "= ci.person_id) ON t.id = ci.movie_id) ON t.id = mk.movie_id) ON k.id = mk.keyword_id WHERE k.keyword "
	            "= 'marvel-cinematic-universe' AND t.production_year > 1990 AND n.gender='m';";
	// RAI
	//	string rai = "CREATE RAI k_mk_t ON movie_keyword (FROM keyword_id REFERENCES keyword.id, TO movie_id REFERENCES
	// title.id);" 	             "CREATE RAI t_ci_n ON cast_info (FROM movie_id REFERENCES title.id, TO person_id
	// REFERENCES name.id);";
	string rai = "";
	string jo_file_dir = "third_party/join_order_test/jos/Q2/";
	const idx_t N_JOS = 1;
	auto file_system = make_unique<FileSystem>();
	// run evaluation
	for (idx_t i = 1; i <= N_JOS; i++) {
		string jo_file = jo_file_dir + to_string(i) + ".json";
		if (!file_system->FileExists(jo_file)) {
			cout << "JO file not exists!" << endl;
			continue;
		}
		ifstream ifs(jo_file);
		string jo_json((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
		cout << "----------------------" << endl;
		cout << "|| "
		     << "Q2-" << to_string(i) << " ||" << endl;
		cout << "----------------------" << endl;
		for (idx_t n = 0; n < N_RUN; n++) {
			RunnerWrapper(q2, rai, jo_json, table_names, n);
			this_thread::sleep_for(chrono::seconds(2));
		}
	}
	// run profiling
	for (idx_t i = 1; i <= N_JOS; i++) {
		string jo_file = jo_file_dir + to_string(i) + ".json";
		if (!file_system->FileExists(jo_file)) {
			cout << "JO file not exists!" << endl;
			continue;
		}
		ifstream ifs(jo_file);
		string jo_json((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
		cout << "----------------------" << endl;
		cout << "|| "
		     << "Q2-" << to_string(i) << " ||" << endl;
		cout << "----------------------" << endl;
		RunnerWrapper(q2, rai, jo_json, table_names, N_RUN);
		this_thread::sleep_for(chrono::seconds(2));
	}
}

void testLDBCCreateRAI() {
	DuckDB db(nullptr);
	Connection con(db);
	auto result = con.Query("BEGIN TRANSACTION");
	for (idx_t i = 0; i < 17; i++) {
		con.Query(ldbc::get_ddl(i));
	}
	con.Query("COMMIT");
	result = con.Query("CREATE PKFK RAI messageHasCreator ON message (FROM m_creatorid REFERENCES person.p_personid, "
	                   "TO m_creatorid REFERENCES person.p_personid);");
	result = con.Query("CREATE PKFK RAI personIsLocatedIn ON person (FROM p_placeid REFERENCES place.pl_placeid, TO "
	                   "p_placeid REFERENCES place.pl_placeid);");
	result = con.Query("CREATE PKFK RAI messageIsLocatedIn ON message (FROM m_locationid REFERENCES place.pl_placeid, "
	                   "TO m_locationid REFERENCES place.pl_placeid);");
	result = con.Query("CREATE PKFK RAI orgIsLocatedIn ON organisation (FROM o_placeid REFERENCES place.pl_placeid, TO "
	                   "o_placeid REFERENCES place.pl_placeid);");
	result = con.Query("CREATE PKFK RAI replyOf ON message (FROM m_c_replyof REFERENCES message.m_messageid, TO "
	                   "m_c_replyof REFERENCES message.m_messageid);");
	result = con.Query("CREATE PKFK RAI postIsPublishedOn ON message (FROM m_ps_forumid REFERENCES forum.f_forumid, TO "
	                   "m_ps_forumid REFERENCES forum.f_forumid);");
	result = con.Query("CREATE PKFK RAI forumHasModerator ON forum (FROM f_moderatorid REFERENCES person.p_personid, "
	                   "TO f_moderatorid REFERENCES person.p_personid);");
	result = con.Query("CREATE UNDIRECTED RAI per_frm ON forum_person (FROM fp_personid REFERENCES person.p_personid, "
	                   "TO fp_forumid REFERENCES forum.f_forumid);");
	result = con.Query("CREATE UNDIRECTED RAI tag_frm ON forum_tag (FROM ft_forumid REFERENCES forum.f_forumid, TO "
	                   "ft_tagid REFERENCES tag.t_tagid);");
	result = con.Query("CREATE UNDIRECTED RAI tag_per ON person_tag (FROM pt_personid REFERENCES person.p_personid, TO "
	                   "pt_tagid REFERENCES tag.t_tagid);");
	result = con.Query("CREATE SELF RAI knows_r ON knows (FROM k_person1id REFERENCES person.p_personid, TO "
	                   "k_person2id REFERENCES person.p_personid);");
	result = con.Query("CREATE UNDIRECTED RAI likes_r ON likes (FROM l_personid REFERENCES person.p_personid, TO "
	                   "l_messageid REFERENCES message.m_messageid);");
	result = con.Query("CREATE UNDIRECTED RAI tag_msg ON message_tag (FROM mt_messageid REFERENCES "
	                   "message.m_messageid, TO mt_tagid REFERENCES tag.t_tagid);");
	result = con.Query("CREATE UNDIRECTED RAI cmg_per ON person_company (FROM pc_personid REFERENCES "
	                   "person.p_personid, TO pc_organisationid REFERENCES organisation.o_organisationid);");
	result = con.Query("CREATE UNDIRECTED RAI unive_per ON person_university (FROM pu_organisationid REFERENCES "
	                   "organisation.o_organisationid, TO pu_personid REFERENCES person.p_personid);");
	con.EnableProfiling();
	for (idx_t i = 1; i <= 16; i++) {
		cout << "====== Q" << i << " ======" << endl;
		auto query = ldbc::get_light_query(i, 1);
		result = con.Query(query);
		cout << con.GetProfilingInformation();
	}
}

void testTPCHCreateRAI() {
	DuckDB db(nullptr);
	Connection con(db);
	tpch::dbgen(0.1, db);
	auto result = con.Query("CREATE PKFK RAI supplierInNation ON supplier (FROM s_nationkey REFERENCES "
	                        "nation.n_nationkey, TO s_nationkey REFERENCES nation.n_nationkey);");
	result->Print();
	result = con.Query("CREATE PKFK RAI customerInNation ON customer (FROM c_nationkey REFERENCES nation.n_nationkey, "
	                   "TO c_nationkey REFERENCES nation.n_nationkey);");
	result->Print();
	result = con.Query("CREATE PKFK RAI customerHasOrders ON orders (FROM o_custkey REFERENCES customer.c_custkey, TO "
	                   "o_custkey REFERENCES customer.c_custkey);");
	result->Print();
	result = con.Query("CREATE PKFK RAI ordersHasLineitems ON lineitem (FROM l_orderkey REFERENCES orders.o_orderkey, "
	                   "TO l_orderkey REFERENCES orders.o_orderkey);");
	result->Print();
	result = con.Query("CREATE RAI supp_part ON partsupp (FROM ps_suppkey REFERENCES supplier.s_suppkey, TO ps_partkey "
	                   "REFERENCES part.p_partkey);");
	result->Print();
	result = con.Query("CREATE RAI order_supp ON lineitem (FROM l_orderkey REFERENCES orders.o_orderkey, TO l_suppkey "
	                   "REFERENCES supplier.s_suppkey);");
	result->Print();
	result = con.Query("CREATE RAI order_part ON lineitem (FROM l_orderkey REFERENCES orders.o_orderkey, TO l_partkey "
	                   "REFERENCES part.p_partkey);");
	result->Print();
	con.EnableProfiling();
	for (idx_t i = 1; i <= 32; i++) {
		cout << "====== Q" << i << " ======" << endl;
		auto query = tpch::get_query(i);
		result = con.Query(query);
		cout << con.GetProfilingInformation();
	}
}

void testLDBCEnumeratedPlans() {
	string ldbc_ic01a = "select p2.id, p2.p_lastname, p2.p_birthday, p2.p_creationdate, p2.p_gender, p2.p_browserused, "
	                    "p2.p_locationip, pl.pl_name from person p1 JOIN (knows JOIN (person p2 JOIN place pl ON "
	                    "p2.p_placeid=pl.pl_placeid) ON k_person2id=p2.p_personid) ON p1.p_personid=k_person1id where "
	                    "p1.id = 2199023255558;";
	string ldbc_ic01b = "select p2.id, p2.p_lastname, p2.p_birthday, p2.p_creationdate, p2.p_gender, p2.p_browserused, "
	                    "p2.p_locationip, pl.pl_name from person p1 JOIN (knows k1 JOIN (knows k2 JOIN (person p2 JOIN "
	                    "place pl ON p2.p_placeid=pl.pl_placeid) ON k2.k_person2id=p2.p_personid) ON "
	                    "k1.k_person2id=k2.k_person1id) ON p1.p_personid=k1.k_person1id where p1.id = 26388279079964";
	string ldbc_ic01c = "select p2.id, p2.p_lastname, p2.p_birthday, p2.p_creationdate, p2.p_gender, p2.p_browserused, "
	                    "p2.p_locationip, pl.pl_name from person p1 JOIN (knows k1 JOIN (knows k2 JOIN (knows k3 JOIN "
	                    "(person p2 JOIN place pl ON p2.p_placeid=pl.pl_placeid) ON k3.k_person2id=p2.p_personid) ON "
	                    "k2.k_person2id=k3.k_person1id) ON k1.k_person2id=k2.k_person1id) ON "
	                    "p1.p_personid=k1.k_person1id where p1.id = 8796093028953 and p2.p_firstname = 'John'";
	string ldbc_ic02 = "select p2.id, p2.p_firstname, p2.p_lastname, message.m_messageid, message.m_content, "
	                   "message.m_creationdate from person p1 JOIN (knows JOIN (person p2 JOIN message ON "
	                   "p2.p_personid=m_creatorid) ON knows.k_person2id=p2.p_personid) ON p1.p_personid=k_person1id "
	                   "where m_creationdate <= '2012-06-04 00:00:00'and p1.id = 15393162793684";
	idx_t plan_nums[] = {40, 224, 1344, 40};
	string query_names[] = {"ic01a", "ic01b", "ic01c", "ic02"};
	string queries[] = {ldbc_ic01a, ldbc_ic01b, ldbc_ic01c, ldbc_ic02};
	DuckDB db(nullptr);
	Connection con(db);
	ldbc::dbgen(con, 1, false, true);
	con.EnableProfiling();
	for (idx_t i = 0; i < 4; i++) {
		cout << "****************************" << endl;
		cout << "****** QUERY " << query_names[i] << " ******" << endl;
		cout << "****************************" << endl;
		vector<idx_t> unpassed_plans;
		auto file_system = make_unique<FileSystem>();
		string jo_file_dir = "/Users/guodong/Developer/graindb-optimizer/ldbc_light/jos/" + query_names[i] + "/";
		idx_t result_num = -1;
		for (idx_t j = 1; j <= plan_nums[i]; j++) {
			string jo_file = jo_file_dir + to_string(j) + ".json";
			if (!file_system->FileExists(jo_file)) {
				cout << "JO file not exists!" << endl;
				continue;
			}
			ifstream ifs(jo_file);
			string jo_json((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
			con.EnableExplicitJoinOrder(jo_json);
			auto result = con.Query(queries[i]);
			cout << "====== PLAN-" << j << endl;
			cout << con.GetProfilingInformation();
			cout << result->ToString() << endl;
			if (result_num == -1) {
				result_num = result->collection.count;
			} else {
				if (result->collection.count != result_num) {
					unpassed_plans.push_back(j);
				}
			}
		}
		cout << "NUM UNPASSED PLANS: " << unpassed_plans.size() << endl;
		for (auto id : unpassed_plans) {
			cout << "UNPASS PLAN " << id << endl;
		}
	}
}

void testLDBCQueries() {
	DuckDB db(nullptr);
	Connection con(db);
	ldbc::dbgen(con, 1, false, true);
	for (idx_t i = 1; i < 26; i++) {
		auto query = ldbc::get_light_query(i, 1);
		auto result = con.Query(query);
		cout << result->ToString() << endl;
	}
}

void GenerateSelectivityForKnows() {
	DuckDB db(nullptr);
	Connection con(db);
	string data_dir = "/home/g35jin/ldbc_dataset/sf30/social_network/relation";
	con.Query(
	    "create table person (p_personid bigint not null, id bigint not null, vid varchar, p_firstname varchar not "
	    "null, p_lastname varchar not null, p_gender int not null, p_birthday varchar not null, p_creationdate bigint "
	    "not null, p_locationip varchar not null, p_browserused int not null, p_placeid bigint );");
	con.Query("create table knows (k_person1id bigint not null, k_person2id bigint not null, k_creationdate bigint not "
	          "null );");
	con.Query("COPY person FROM '" + data_dir + "/person.csv' WITH ( DELIMITER ',', HEADER 1, ESCAPE '\\');");
	con.Query("COPY knows FROM '" + data_dir +
	          "/person_knows_person.csv' WITH ( DELIMITER ',', HEADER 1, ESCAPE '\\');");
	auto result3 = con.Query("SELECT COUNT(*) FROM knows;");
	auto total_knows = result3->GetValue(0, 0).value_.bigint;
	auto result4 = con.Query("SELECT COUNT(*) FROM person;");
	auto total_person = result4->GetValue(0, 0).value_.bigint;
	cout << "Num of person : " << total_person << endl;
	cout << "Num of knows: " << total_knows << endl;
	for (int i = 0; i < total_person;) {
		string query_template = "SELECT COUNT(*) FROM person p1, knows k, person p2 WHERE p1.p_personid=k.k_person1id "
		                        "AND k.k_person2id=p2.p_personid AND p1.rowid<=" +
		                        to_string(i);
		auto result = con.Query(query_template);
		auto num = result->GetValue(0, 0).value_.bigint;
		i += 10;
		cout << i << "," << num << "," << ((double)num / (double)total_knows) << endl;
	}
}

void GenerateSelectivityForLikesComment() {
	DuckDB db(nullptr);
	Connection con(db);
	string data_dir = "/home/g35jin/ldbc_dataset/sf30/social_network/relation";
	con.Query(
	    "create table person (p_personid bigint not null, id bigint not null, vid varchar, p_firstname varchar not "
	    "null, p_lastname varchar not null, p_gender int not null, p_birthday varchar not null, p_creationdate bigint "
	    "not null, p_locationip varchar not null, p_browserused int not null, p_placeid bigint );");
	con.Query("create table likes_comment (l_personid bigint not null, l_messageid bigint not null, l_creationdate  "
	          "bigint not null );");
	con.Query("COPY person FROM '" + data_dir + "/person.csv' WITH ( DELIMITER ',', HEADER 1, ESCAPE '\\');");
	con.Query("COPY likes_comment FROM '" + data_dir +
	          "/likes_comment.csv' WITH ( DELIMITER ',', HEADER 1, ESCAPE '\\');");
	auto result3 = con.Query("SELECT COUNT(*) FROM likes_comment;");
	auto total_edges = result3->GetValue(0, 0).value_.bigint;
	auto result4 = con.Query("SELECT COUNT(*) FROM person;");
	auto total_person = result4->GetValue(0, 0).value_.bigint;
	cout << "Num of person : " << total_person << endl;
	cout << "Num of edges: " << total_edges << endl;
	for (int i = 0; i < total_person;) {
		string query_template =
		    "SELECT COUNT(*) FROM person p1, likes_comment k, WHERE p1.p_personid=k.l_personid AND p1.rowid<=" +
		    to_string(i);
		auto result = con.Query(query_template);
		auto num = result->GetValue(0, 0).value_.bigint;
		i += 10;
		cout << i << "," << num << "," << ((double)num / (double)total_edges) << endl;
	}
}

void GenerateSelectivityForumPost(int step) {
	DuckDB db(nullptr);
	Connection con(db);
	string data_dir = "/home/g35jin/ldbc_dataset/sf30/social_network/relation";
	con.Query("create table post (ps_id bigint not null, id bigint not null, vid varchar, ps_imagefile varchar, "
	          "ps_creationdate bigint not null, ps_locationip varchar not null, ps_browserused int not null, "
	          "ps_language varchar, ps_content text, ps_length int not null, ps_creatorid bigint, ps_forumid bigint, "
	          "ps_locationid bigint );");
	con.Query("create table forum (f_forumid bigint not null, id bigint not null, vid varchar, f_title varchar not "
	          "null, f_creationdate bigint not null, f_moderatorid bigint );");
	con.Query("COPY post FROM '" + data_dir + "/post.csv' WITH ( DELIMITER ',', HEADER 1, ESCAPE '\\');");
	con.Query("COPY forum FROM '" + data_dir + "/forum.csv' WITH ( DELIMITER ',', HEADER 1, ESCAPE '\\');");
	auto result3 = con.Query("SELECT COUNT(*) FROM post;");
	auto total_post = result3->GetValue(0, 0).value_.bigint;
	auto result4 = con.Query("SELECT COUNT(*) FROM forum;");
	auto total_forum = result4->GetValue(0, 0).value_.bigint;
	cout << "Step: " << step << endl;
	cout << "Num of post : " << total_post << endl;
	cout << "Num of forum: " << total_forum << endl;
	for (int i = 0; i < total_forum;) {
		string query_template =
		    "SELECT COUNT(*) FROM forum f JOIN post p ON p.ps_forumid=f.f_forumid WHERE f.rowid<=" + to_string(i);
		auto result = con.Query(query_template);
		auto num = result->GetValue(0, 0).value_.bigint;
		i += step;
		cout << i << "," << num << "," << ((double)num / (double)total_post) << endl;
	}
}

void GenerateSelectivityLikesCmtDate(int step) {
	DuckDB db(nullptr);
	Connection con(db);
	string data_dir = "/home/g35jin/ldbc_dataset/sf30/social_network/relation";
	con.Query(
	    "create table person (p_personid bigint not null, id bigint not null, vid varchar, p_firstname varchar not "
	    "null, p_lastname varchar not null, p_gender int not null, p_birthday varchar not null, p_creationdate bigint "
	    "not null, p_locationip varchar not null, p_browserused int not null, p_placeid bigint );");
	con.Query(
	    "create table person_likes_comment (l_personid bigint not null, l_messageid bigint not null, l_creationdate  "
	    "bigint not null );");
	con.Query("COPY person FROM '" + data_dir + "/person.csv' WITH ( DELIMITER ',', HEADER 1, ESCAPE '\\');");
	con.Query("COPY likes_comment FROM '" + data_dir +
	          "/likes_comment.csv' WITH ( DELIMITER ',', HEADER 1, ESCAPE '\\');");
	auto numLikesRes = con.Query("SELECT COUNT(*) FROM likes_comment;");
	auto numLikes = numLikesRes->GetValue(0, 0).value_.bigint;
	auto minLikesDateRes = con.Query("SELECT MIN(l_creationdate) FROM likes_comment;");
	auto minLikesDate = minLikesDateRes->GetValue(0, 0).value_.bigint;
	auto maxLikesDateRes = con.Query("SELECT MAX(l_creationdate) FROM likes_comment;");
	auto maxLikesDate = maxLikesDateRes->GetValue(0, 0).value_.bigint;
	cout << "Step: " << step << endl;
	for (int i = minLikesDate; i < maxLikesDate;) {
		string query_template =
		    "SELECT COUNT(*) FROM person p JOIN likes_comment l ON p.p_personid=l.l_personid WHERE l.l_creationdate<=" +
		    to_string(i);
		auto result = con.Query(query_template);
		auto num = result->GetValue(0, 0).value_.bigint;
		i += step;
		cout << i << "," << num << "," << ((double)num / (double)numLikes) << endl;
	}
}

void GenerateSelectivityKnowsDate(int step) {
	DuckDB db(nullptr);
	Connection con(db);
	string data_dir = "/home/g35jin/ldbc_dataset/sf30/social_network/relation";
	con.Query(
	    "create table person (p_personid bigint not null, id bigint not null, vid varchar, p_firstname varchar not "
	    "null, p_lastname varchar not null, p_gender int not null, p_birthday varchar not null, p_creationdate bigint "
	    "not null, p_locationip varchar not null, p_browserused int not null, p_placeid bigint );");
	con.Query("create table knows (k_person1id bigint not null, k_person2id bigint not null, k_creationdate bigint not "
	          "null );");
	con.Query("COPY person FROM '" + data_dir + "/person.csv' WITH ( DELIMITER ',', HEADER 1, ESCAPE '\\');");
	con.Query("COPY knows FROM '" + data_dir +
	          "/person_knows_person.csv' WITH ( DELIMITER ',', HEADER 1, ESCAPE '\\');");
	auto numKnowsRes = con.Query("SELECT COUNT(*) FROM knows;");
	auto numKnows = numKnowsRes->GetValue(0, 0).value_.bigint;
	auto minKnowsDateRes = con.Query("SELECT MIN(k_creationdate) FROM knows;");
	auto minKnowsDate = minKnowsDateRes->GetValue(0, 0).value_.bigint;
	auto maxKnowsDateRes = con.Query("SELECT MAX(k_creationdate) FROM knows;");
	auto maxKnowsDate = maxKnowsDateRes->GetValue(0, 0).value_.bigint;
	cout << "Step: " << step << endl;
	for (int i = minKnowsDate; i < maxKnowsDate;) {
		string query_template =
		    "SELECT COUNT(*) FROM person p JOIN knows k ON k.k_person1id = p.p_personid WHERE k.k_creationdate<=" +
		    to_string(i);
		auto result = con.Query(query_template);
		auto num = result->GetValue(0, 0).value_.bigint;
		i += step;
		cout << i << "," << num << "," << ((double)num / (double)numKnows) << endl;
	}
}

void TestSIPThreshold() {
	DuckDB db(nullptr);
	Connection conn(db);
	conn.Query("CREATE TABLE person(id INTEGER, name VARCHAR, placeid INTEGER);");
	conn.Query("CREATE TABLE likes(pid INTEGER, mid INTEGER);");
	conn.Query("CREATE TABLE knows(p1id INTEGER, p2id INTEGER);");
	conn.Query("CREATE TABLE message(id INTEGER, content VARCHAR);");
	conn.Query("CREATE TABLE place(id INTEGER, country VARCHAR);");
	conn.Query("COPY person FROM '/Users/guodong/Developer/graindb/dataset/tiny/person.csv' (HEADER);");
	conn.Query("COPY likes FROM '/Users/guodong/Developer/graindb/dataset/tiny/likes.csv' (HEADER);");
	conn.Query("COPY knows FROM '/Users/guodong/Developer/graindb/dataset/tiny/knows.csv' (HEADER);");
	conn.Query("COPY message FROM '/Users/guodong/Developer/graindb/dataset/tiny/message.csv' (HEADER);");
	conn.Query("COPY place FROM '/Users/guodong/Developer/graindb/dataset/tiny/place.csv' (HEADER);");
	conn.Query(
	    "CREATE UNDIRECTED RAI likes_rai ON likes(FROM pid REFERENCES person.id, TO mid REFERENCES message.id);");
	conn.Query(
	    "CREATE PKFK RAI person_place ON person(FROM placeid REFERENCES place.id, TO placeid REFERENCES place.id);");
	conn.Query("CREATE SELF RAI knows_rai ON knows(FROM p1id REFERENCES person.id, TO p2id REFERENCES person.id);");
	conn.EnableExplicitJoinOrder(
	    "{\"type\": \"JOIN\", \"children\": [{\"type\": \"SCAN\", \"table\": \"m\"}, {\"type\": \"JOIN\", "
	    "\"children\": [{\"type\": \"SCAN\", \"table\": \"l\"}, {\"type\": \"SCAN\", \"table\": \"p\"} ] } ] }");
	auto result =
	    conn.Query("SELECT COUNT(*) FROM person p, likes l, message m WHERE l.pid=p.id AND l.mid=m.id AND p.id=2;");
	conn.DisableExplicitJoinOrder();
	cout << result->ToString();
	result = conn.Query("SELECT COUNT(*) FROM person p, place pl WHERE p.placeid=pl.id AND pl.id=0;");
	cout << result->ToString();
	conn.EnableExplicitJoinOrder(
	    "{\"type\": \"JOIN\", \"children\": [{\"type\": \"SCAN\", \"table\": \"pl\"}, {\"type\": \"JOIN\", "
	    "\"children\": [{\"type\": \"SCAN\", \"table\": \"p\"}, {\"type\": \"JOIN\", \"children\": [{\"type\": "
	    "\"SCAN\", \"table\": \"l\"}, {\"type\": \"SCAN\", \"table\": \"m\"} ] } ] } ] }");
	result = conn.Query("SELECT COUNT(*) FROM person p, likes l, message m, place pl WHERE l.pid=p.id AND l.mid=m.id "
	                    "AND p.placeid=pl.id AND p.id=5;");
	cout << result->ToString();
	conn.DisableExplicitJoinOrder();
	result = conn.Query("SELECT COUNT(*) FROM knows k1, knows k2 WHERE k1.p2id=k2.p1id AND k1.p1id=0;");
	cout << result->ToString();
}

void runJOBProfiler() {
	DuckDB db(nullptr);
	Connection con(db);
	imdb::dbgen(con, true, true);
	con.EnableProfiling();
	for (idx_t i = 1; i <= 113; i++) {
		auto query = imdb::get_113_query(i);
		string q_name = "Q";
		if (i < 10) {
			q_name += "00";
		} else if (i < 100) {
			q_name += "0";
		}
		q_name += to_string(i);
		string jo_file = q_name + ".json";
		auto jos = imdb::get_113_optimized_jo(jo_file);
		con.EnableExplicitJoinOrder(jos);
		auto result = con.Query(query);
		cout << "-------------------------------" << endl;
		cout << "|| IMDB_113_OPTIMIZED_" << q_name << " ||" << endl;
		cout << "-------------------------------" << endl;
		cout << con.GetProfilingInformation(ProfilerPrintFormat::JSON);
	}
}

int main(int argc, char *argv[]) {
	runJOBProfiler();
	//	TestSIPThreshold();
	//	GenerateSelectivityKnowsDate(stoi(argv[1]));
	// GenerateSelectivityLikesCmtDate(1000);
	//	GenerateSelectivityForumPost(10);
	//	testTPCHCreateRAI();
	//	testJOBEnumeratedPlans();
	//	testLDBCQueries();
	//	testLDBCEnumeratedPlans();
}
