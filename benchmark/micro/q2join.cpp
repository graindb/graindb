#include "benchmark_runner.hpp"
#include "duckdb_benchmark_macro.hpp"

#define SF 30

using namespace duckdb;
using namespace std;

DUCKDB_BENCHMARK(Q2JoinUnSortedWithoutIndex, "[micro]")
virtual void Load(DuckDBBenchmarkState *state) {
	state->conn.Query("CREATE TABLE person(name VARCHAR, gender VARCHAR, age INTEGER, creationDate BIGINT, locationIP "
	                  "VARCHAR, browserUsed VARCHAR, place BIGINT);");
	state->conn.Query("CREATE TABLE know(p1name VARCHAR, p2name VARCHAR, creationDate BIGINT);");
	// TODO: the path should not be hard-coded. Fix this later.
	state->conn.Query("COPY person FROM '/home/g35jin/ldbc_dataset/sf30/person_names.csv' HEADER DELIMITER '|';");
	state->conn.Query("COPY know FROM '/home/g35jin/ldbc_dataset/sf30/knows_shuffle.csv' HEADER DELIMITER '|';");
}

virtual string GetQuery() {
	return "SELECT p1.name, p1.gender, p2.name, p2.gender FROM person p1, person p2, know WHERE p1.name=know.p1name "
	       "AND p2.name=know.p2name;";
}

virtual string VerifyResult(QueryResult *result) {
	if (!result->success) {
		return result->error;
	}
	return string();
}

virtual string BenchmarkInfo() {
	return StringUtil::Format("Runs the following query: \"SELECT p1.name, p1.gender, p2.name, p2.gender FROM person "
	                          "p1, person p2, know WHERE p1.name=know.p1name AND p2.name=know.p2name;\""
	                          " on scale factor %d",
	                          SF);
}
FINISH_BENCHMARK(Q2JoinUnSortedWithoutIndex)

DUCKDB_BENCHMARK(Q2JoinUnSortedWithIndex, "[micro]")
virtual void Load(DuckDBBenchmarkState *state) {
	state->conn.Query("CREATE TABLE person(name VARCHAR, gender VARCHAR, age INTEGER, creationDate BIGINT, locationIP "
	                  "VARCHAR, browserUsed VARCHAR, place BIGINT);");
	state->conn.Query("CREATE TABLE know(p1name VARCHAR, p2name VARCHAR, creationDate BIGINT);");
	// TODO: the path should not be hard-coded. Fix this later.
	state->conn.Query("COPY person FROM '/home/g35jin/ldbc_dataset/sf30/person_names.csv' HEADER DELIMITER '|';");
	state->conn.Query("COPY know FROM '/home/g35jin/ldbc_dataset/sf30/knows_shuffle.csv' HEADER DELIMITER '|';");
	state->conn.Query("CREATE INDEX know_p1name_idx ON know(p1name);");
	state->conn.Query("CREATE INDEX person_name ON person(name);");
}

virtual string GetQuery() {
	return "SELECT p1.name, p1.gender, p2.name, p2.gender FROM person p1, person p2, know WHERE p1.name=know.p1name "
	       "AND p2.name=know.p2name;";
}

virtual string VerifyResult(QueryResult *result) {
	if (!result->success) {
		return result->error;
	}
	return string();
}

virtual string BenchmarkInfo() {
	return StringUtil::Format("Runs the following query: \"SELECT p1.name, p1.gender, p2.name, p2.gender FROM person "
	                          "p1, person p2, know WHERE p1.name=know.p1name AND p2.name=know.p2name;\""
	                          " on scale factor %d",
	                          SF);
}
FINISH_BENCHMARK(Q2JoinUnSortedWithIndex)

DUCKDB_BENCHMARK(Q2JoinSortedWithoutIndex, "[micro]")
virtual void Load(DuckDBBenchmarkState *state) {
	state->conn.Query("CREATE TABLE person(name VARCHAR, gender VARCHAR, age INTEGER, creationDate BIGINT, locationIP "
	                  "VARCHAR, browserUsed VARCHAR, place BIGINT);");
	state->conn.Query("CREATE TABLE know(p1name VARCHAR, p2name VARCHAR, creationDate BIGINT);");
	// TODO: the path should not be hard-coded. Fix this later.
	state->conn.Query("COPY person FROM '/home/g35jin/ldbc_dataset/sf30/person_names.csv' HEADER DELIMITER '|';");
	state->conn.Query("COPY know FROM '/home/g35jin/ldbc_dataset/sf30/knows_names.csv' HEADER DELIMITER '|';");
}

virtual string GetQuery() {
	return "SELECT p1.name, p1.gender, p2.name, p2.gender FROM person p1, person p2, know WHERE p1.name=know.p1name "
	       "AND p2.name=know.p2name;";
}

virtual string VerifyResult(QueryResult *result) {
	if (!result->success) {
		return result->error;
	}
	return string();
}

virtual string BenchmarkInfo() {
	return StringUtil::Format("Runs the following query: \"SELECT p1.name, p1.gender, p2.name, p2.gender FROM person "
	                          "p1, person p2, know WHERE p1.name=know.p1name AND p2.name=know.p2name;\""
	                          " on scale factor %d",
	                          SF);
}
FINISH_BENCHMARK(Q2JoinSortedWithoutIndex)

DUCKDB_BENCHMARK(Q2JoinSortedWithIndex, "[micro]")
virtual void Load(DuckDBBenchmarkState *state) {
	state->conn.Query("CREATE TABLE person(name VARCHAR, gender VARCHAR, age INTEGER, creationDate BIGINT, locationIP "
	                  "VARCHAR, browserUsed VARCHAR, place BIGINT);");
	state->conn.Query("CREATE TABLE know(p1name VARCHAR, p2name VARCHAR, creationDate BIGINT);");
	// TODO: the path should not be hard-coded. Fix this later.
	state->conn.Query("COPY person FROM '/home/g35jin/ldbc_dataset/sf30/person_names.csv' HEADER DELIMITER '|';");
	state->conn.Query("COPY know FROM '/home/g35jin/ldbc_dataset/sf30/knows_shuffle.csv' HEADER DELIMITER '|';");
	state->conn.Query("CREATE INDEX know_p1name_idx ON know(p1name);");
	state->conn.Query("CREATE INDEX person_name ON person(name);");
}

virtual string GetQuery() {
	return "SELECT p1.name, p1.gender, p2.name, p2.gender FROM person p1, person p2, know WHERE p1.name=know.p1name "
	       "AND p2.name=know.p2name;";
}

virtual string VerifyResult(QueryResult *result) {
	if (!result->success) {
		return result->error;
	}
	return string();
}

virtual string BenchmarkInfo() {
	return StringUtil::Format("Runs the following query: \"SELECT p1.name, p1.gender, p2.name, p2.gender FROM person "
	                          "p1, person p2, know WHERE p1.name=know.p1name AND p2.name=know.p2name;\""
	                          " on scale factor %d",
	                          SF);
}
FINISH_BENCHMARK(Q2JoinSortedWithIndex)
