//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/execution/operator/helper/physical_vacuum.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/execution/physical_operator.hpp"
#include "duckdb/parser/parsed_data/vacuum_info.hpp"

namespace duckdb {

//! PhysicalVacuum represents a VACUUM operation (e.g. VACUUM or ANALYZE)
class PhysicalVacuum : public PhysicalOperator {
public:
	PhysicalVacuum(unique_ptr<VacuumInfo> info)
	    : PhysicalOperator(PhysicalOperatorType::VACUUM, {TypeId::BOOL}), info(move(info)) {
	}

	unique_ptr<VacuumInfo> info;

public:
	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;
};

/* ----------------------
 *		Vacuum and Analyze Statements
 *
 * Even though these are nominally two statements, it's convenient to use
 * just one node type for both.  Note that at least one of PG_VACOPT_VACUUM
 * and PG_VACOPT_ANALYZE must be set in options.
 * ----------------------
 */
typedef enum PGVacuumOption {
	PG_VACOPT_VACUUM = 1 << 0,               /* do VACUUM */
	PG_VACOPT_ANALYZE = 1 << 1,              /* do ANALYZE */
	PG_VACOPT_VERBOSE = 1 << 2,              /* print progress info */
	PG_VACOPT_FREEZE = 1 << 3,               /* FREEZE option */
	PG_VACOPT_FULL = 1 << 4,                 /* FULL (non-concurrent) vacuum */
	PG_VACOPT_NOWAIT = 1 << 5,               /* don't wait to get lock (autovacuum only) */
	PG_VACOPT_SKIPTOAST = 1 << 6,            /* don't process the TOAST table, if any */
	PG_VACOPT_DISABLE_PAGE_SKIPPING = 1 << 7 /* don't skip any pages */
} PGVacuumOption;

} // namespace duckdb
