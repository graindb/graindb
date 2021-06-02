//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/execution/operator/scan/physical_table_scan.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/execution/physical_operator.hpp"
#include "duckdb/storage/data_table.hpp"

namespace duckdb {

//! Represents a scan of a base table
class PhysicalTableScan : public PhysicalOperator {
public:
	PhysicalTableScan(LogicalOperator &op, TableCatalogEntry &tableref, idx_t table_index, DataTable &table,
	                  vector<column_t> column_ids, vector<unique_ptr<Expression>> filter,
	                  unordered_map<idx_t, vector<TableFilter>> table_filters);

	//! The table to scan
	TableCatalogEntry &tableref;
	//! The table id referenced in logical plan
	idx_t table_index;
	//! The physical data table to scan
	DataTable &table;
	//! The column ids to project
	vector<column_t> column_ids;

	//! The filter expression
	unique_ptr<Expression> expression;
	//! Filters pushed down to table scan
	unordered_map<idx_t, vector<TableFilter>> table_filters;

	// extra_info
	bool seq_scan = true;
	idx_t lookup_size = 0;

public:
	bool PushdownZoneFilter(idx_t table_index, const shared_ptr<bitmask_vector> &zone_filter,
	                        const shared_ptr<bitmask_vector> &zone_sel) override;
	bool PushdownRowsFilter(idx_t table_index, const shared_ptr<rows_vector> &rows_filter, idx_t count) override;
	void PerformSeqScan(DataChunk &chunk, PhysicalOperatorState *state_, Transaction &transaction);
	void PerformLookup(DataChunk &chunk, PhysicalOperatorState *state_, SelectionVector *sel, Vector *rid_vector,
	                   DataChunk *rai_chunk, Transaction &transaction);
	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;
	string ExtraRenderInformation() const override;
	unique_ptr<PhysicalOperatorState> GetOperatorState() override;

private:
	//! The rows filter
	shared_ptr<rows_vector> rows_filter;
	shared_ptr<bitmask_vector> row_bitmask;
	shared_ptr<bitmask_vector> zone_bitmask;
	row_t rows_count;
};

} // namespace duckdb
