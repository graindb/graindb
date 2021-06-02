//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/execution/operator/scan/physical_fetch_scan.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/execution/physical_operator.hpp"
#include "duckdb/storage/data_table.hpp"

namespace duckdb {

class PhysicalLookupOperatorState : public PhysicalOperatorState {
public:
	PhysicalLookupOperatorState() : PhysicalOperatorState(nullptr), initialized(false) {
	}
	explicit PhysicalLookupOperatorState(Expression &expr)
	    : PhysicalOperatorState(nullptr), initialized(false), executor(expr) {
	}

	//! Whether or not the scan has been initialized
	bool initialized;
	//! The current position in the scan
	TableScanState scan_offset;
	//! Execute filters inside the table
	ExpressionExecutor executor;
	TableIndexScanState index_state;
};

class PhysicalLookup : public PhysicalOperator {
public:
	PhysicalLookup(LogicalOperator &op, TableCatalogEntry &tableref, idx_t table_index, DataTable &table,
	               vector<column_t> &column_ids, vector<unique_ptr<Expression>> filter,
	               unordered_map<idx_t, vector<TableFilter>> table_filters);

	TableCatalogEntry &tableref;
	//! The table id referenced in logical plan
	idx_t table_index;
	DataTable &table;
	vector<column_t> column_ids;

	//! The filter expression
	unique_ptr<Expression> expression;
	//! Filters pushed down to table scan
	unordered_map<idx_t, vector<TableFilter>> table_filters;

	row_t cache_row_id = -1;
	vector<unordered_map<idx_t, data_ptr_t>> segment_ptrs_map;

public:
	template <class T>
	void inline Lookup(ClientContext &context, ColumnData &column, row_t *row_ids, Vector &result, idx_t count,
	                   unordered_map<idx_t, data_ptr_t> &segment_ptrs, idx_t type_size = sizeof(T));
	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;
	string ExtraRenderInformation() const override;
	unique_ptr<PhysicalOperatorState> GetOperatorState() override;
};

} // namespace duckdb
