#include "duckdb/parser/statement/vacuum_statement.hpp"
#include "duckdb/parser/transformer.hpp"

using namespace duckdb;
using namespace std;

unique_ptr<VacuumStatement> Transformer::TransformVacuum(PGNode *node) {
	auto stmt = reinterpret_cast<PGVacuumStmt *>(node);
	assert(stmt);
	auto result = make_unique<VacuumStatement>();
	auto info = make_unique<VacuumInfo>();
	info->vaccum_type = stmt->options;
	result->info = move(info);
	return result;
}
