#include "benchmark_runner.hpp"
#include "duckdb_benchmark_macro.hpp"

using namespace duckdb;
using namespace std;

#define SF 30

DUCKDB_BENCHMARK(Q1JoinUnSortedWithoutIndex, "[micro]")
virtual void Load(DuckDBBenchmarkState *state) {
	state->conn.Query("CREATE TABLE person(name VARCHAR, gender VARCHAR, age INTEGER, creationDate BIGINT, locationIP "
	                  "VARCHAR, browserUsed VARCHAR, place BIGINT);");
	state->conn.Query("CREATE TABLE know(p1name VARCHAR, p2name VARCHAR, creationDate BIGINT);");
	// TODO: the path should not be hard-coded. Fix this later.
	state->conn.Query("COPY person FROM '/home/g35jin/ldbc_dataset/sf30/person_names.csv' HEADER DELIMITER '|';");
	state->conn.Query("COPY know FROM '/home/g35jin/ldbc_dataset/sf30/knows_shuffle.csv' HEADER DELIMITER '|';");
}

virtual string GetQuery() {
	return "SELECT name, gender, p2name FROM person, know WHERE know.p1name=name;";
}

virtual string VerifyResult(QueryResult *result) {
	if (!result->success) {
		return result->error;
	}
	return string();
}

virtual string BenchmarkInfo() {
	return StringUtil::Format("Runs the following query: \"SELECT name, gender, p2name FROM "
	                          "person, know WHERE know.p1name=name;\""
	                          " on scale factor %d sf",
	                          SF);
}
FINISH_BENCHMARK(Q1JoinUnSortedWithoutIndex)

DUCKDB_BENCHMARK(Q1JoinUnSortedWithIndex, "[micro]")
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
	return "SELECT name, gender, p2name FROM person, know WHERE know.p1name=name;";
}

virtual string VerifyResult(QueryResult *result) {
	if (!result->success) {
		return result->error;
	}
	return string();
}

virtual string BenchmarkInfo() {
	return StringUtil::Format(
	    "Runs the following query: \"SELECT name, gender, p2name FROM person, know WHERE know.p1name=name;\""
	    " on scale factor %d",
	    SF);
}
FINISH_BENCHMARK(Q1JoinUnSortedWithIndex)

DUCKDB_BENCHMARK(Q1JoinSortedWithoutIndex, "[micro]")
virtual void Load(DuckDBBenchmarkState *state) {
	state->conn.Query("CREATE TABLE person(name VARCHAR, gender VARCHAR, age INTEGER, creationDate BIGINT, locationIP "
	                  "VARCHAR, browserUsed VARCHAR, place BIGINT);");
	state->conn.Query("CREATE TABLE know(p1name VARCHAR, p2name VARCHAR, creationDate BIGINT);");
	// TODO: the path should not be hard-coded. Fix this later.
	state->conn.Query("COPY person FROM '/home/g35jin/ldbc_dataset/sf30/person_names.csv' HEADER DELIMITER '|';");
	state->conn.Query("COPY know FROM '/home/g35jin/ldbc_dataset/sf30/knows_names.csv' HEADER DELIMITER '|';");
}

virtual string GetQuery() {
	return "SELECT name, gender, p2name FROM person, know WHERE know.p1name=name;";
}

virtual string VerifyResult(QueryResult *result) {
	if (!result->success) {
		return result->error;
	}
	return string();
}

virtual string BenchmarkInfo() {
	return StringUtil::Format(
	    "Runs the following query: \"SELECT name, gender, p2name FROM person, know WHERE know.p1name=name;\""
	    "on scale factor %d",
	    SF);
}
FINISH_BENCHMARK(Q1JoinSortedWithoutIndex)

DUCKDB_BENCHMARK(Q1JoinSortedWithIndex, "[micro]")
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
	return "SELECT name, gender, p2name FROM person, know WHERE know.p1name=name;";
}

virtual string VerifyResult(QueryResult *result) {
	if (!result->success) {
		return result->error;
	}
	return string();
}

virtual string BenchmarkInfo() {
	return StringUtil::Format(
	    "Runs the following query: \"SELECT name, gender, p2name FROM person, know WHERE know.p1name=name;\""
	    " on scale factor %d",
	    SF);
}
FINISH_BENCHMARK(Q1JoinSortedWithIndex)
