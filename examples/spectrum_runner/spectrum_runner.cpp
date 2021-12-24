#include "duckdb.hpp"
#include "duckdb/catalog/catalog_entry/schema_catalog_entry.hpp"
#include "duckdb/main/client_context.hpp"
#include "duckdb/planner/planner.hpp"
#include "duckdb/storage/data_table.hpp"
#include "duckdb/transaction/transaction.hpp"
#include "imdb.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <utility>

#define NUM_RUN_TIMES 5

using namespace std;
using namespace duckdb;

struct QueryRunResult {
public:
	QueryRunResult(int query_id, int jos_id, bool enable_rai, idx_t num_tuples, double elapsed_time_ms)
	    : query_id(query_id), jos_id(jos_id), enable_rai(enable_rai), num_tuples(num_tuples),
	      elapsed_time_ms(elapsed_time_ms) {
	}

	int query_id;
	int jos_id;
	bool enable_rai;
	idx_t num_tuples;
	double elapsed_time_ms;
};

class SpectrumRunner {
public:
	SpectrumRunner(string jos_input, int query_id, int start_jos_id, int num_jos)
	    : jos_input(move(jos_input)), query_id(query_id), start_jos_id(start_jos_id), num_jos(num_jos) {
	}

	void Run() {
		cout << "Start running ... params: [" << jos_input << ", " << query_id << ", " << start_jos_id << ", "
		     << num_jos << "]" << endl;
		string query = GetQuery();
		vector<QueryRunResult> query_results;
		cout << "-------------------------------" << endl;
		cout << "||    QUERY " << query_id << "    ||" << endl;
		cout << "-------------------------------" << endl;
		cout << "jos_id,graindb,num_tuples,elapsed_time" << endl;
		// Run query without rai
		for (idx_t i = start_jos_id; i < num_jos; i++) {
			DuckDB db(nullptr);
			Connection conn(db);
			Initialize(conn, false);
			auto query_result = RunQueryWithAJos(conn, query, i, false);
			cout << query_result.jos_id << "," << (query_result.enable_rai ? "T," : "F,") << query_result.num_tuples
			     << "," << query_result.elapsed_time_ms << endl;
			query_results.push_back(query_result);
		}
		// Run query with rai
		for (idx_t i = start_jos_id; i < num_jos; i++) {
			DuckDB db(nullptr);
			Connection conn(db);
			Initialize(conn, true);
			auto query_result = RunQueryWithAJos(conn, query, i, true);
			cout << query_result.jos_id << "," << (query_result.enable_rai ? "T," : "F,") << query_result.num_tuples
			     << "," << query_result.elapsed_time_ms << endl;
			query_results.push_back(query_result);
		}
	}

private:
	virtual void Initialize(Connection &conn_, bool enableRAI) {
	}

	virtual string GetQuery() {
		return "";
	}

	virtual string GetJos(int jos_id) {
		auto file_system = make_unique<FileSystem>();
		string jo_file;
		if (StringUtil::EndsWith(jos_input, "/")) {
			jo_file = jos_input + to_string(jos_id) + ".json";
		} else {
			jo_file = jos_input + "/" + to_string(jos_id) + ".json";
		}
		if (!file_system->FileExists(jo_file)) {
			cout << "JO file not exists!" << endl;
			return "";
		}
		ifstream ifs(jo_file);
		string jo_json((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
		return jo_json;
	}

	QueryRunResult RunQueryWithAJos(Connection &conn, string &query, int jos_id, bool enable_rai) {
		string jos = GetJos(jos_id);
		idx_t num_tuples = 0;
		vector<double> elapsed_times;
		for (int i = 0; i < NUM_RUN_TIMES; i++) {
			Profiler profiler;
			profiler.Start();
			conn.EnableExplicitJoinOrder(jos);
			auto result = conn.Query(query);
			num_tuples = result->collection.count;
			profiler.End();
			elapsed_times.push_back(profiler.Elapsed());
		}
		if (elapsed_times.size() == 1) {
			return QueryRunResult{query_id, jos_id, enable_rai, num_tuples, elapsed_times[0]};
		} else {
			double elapsed_time = numeric_limits<double>::max();
			for (idx_t k = 1; k < elapsed_times.size(); k++) {
				if (elapsed_times[k] < elapsed_time) {
					elapsed_time = elapsed_times[k];
				}
			}
			return QueryRunResult{query_id, jos_id, enable_rai, num_tuples, elapsed_time};
		}
	}

protected:
	string jos_input;
	int query_id;
	int start_jos_id;
	int num_jos;
};

class JOBSpectrumRunner : public SpectrumRunner {
public:
	JOBSpectrumRunner(string jos_input, int query_id, int start_jos_id, int num_jos)
	    : SpectrumRunner(move(jos_input), query_id, start_jos_id, num_jos) {
	}

private:
	void Initialize(Connection &conn_, bool enableRAI) override {
		imdb::dbgen(conn_, enableRAI, true);
	}

	string GetQuery() override {
		return imdb::get_113_query(query_id);
	}
};

// Q-1: 1-224
// Q-5: 1-244
// Q-9: 1-40
// Q-12: 1-244
// Q-15: 1-244
// Q-18: 1-244
int main(int argc, char *argv[]) {
	if (argc != 5) {
		cout << "USAGE: <JOS_INPUT_DIR>, <QUERY_ID>, <START_JOS_ID>, <END_JOS_ID (not included)>" << endl;
	}
	string jos_input = argv[1];
	int query_id = stoi(argv[2]);
	int start_jos_id = stoi(argv[3]);
	int num_jos = stoi(argv[4]);
	JOBSpectrumRunner job_spectrum_runner(jos_input, query_id, start_jos_id, num_jos);
	job_spectrum_runner.Run();
}

// QG1
// + Q1a: ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q01/ 1 1 225
// + Q1b: ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q01/ 2 1 225
// * Q1c: ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q01/ 3 1 225
// * Q1d: ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q01/ 4 1 255

// QG2
// + Q2a: ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q02/ 5 1 225
// + Q2b: ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q02/ 6 1 225
// STOPPED Q2c: ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q02/ 7 1
// Q2d: ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q02/ 8 1 2

// QG3
// + Q3a: ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q03/ 9 1 41
// + Q3b: ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q03/ 10 1 41
// + Q3c: ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q03/ 11 1 41

// QG4
// + Q4a: ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q04/ 12 1 225
// + Q4b: ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q04/ 13 1 225
// * Q4c: ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q04/ 14 1 225

// QG5
// + ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q05/ 15 1 225
// + ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q05/ 16 1 225
// + ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q05/ 17 1 255

// QG6
// + ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q06/ 18 1 225
// * ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q06/ 19 1 225
// * ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q06/ 20 1 255
// * ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q06/ 21 1 255
// ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q06/ 22 1 2
// ./build/release/examples/spectrum_runner/spectrum_runner /home/g35jin/graindb-optimizer/job/jos/Q06/ 23 1 2
