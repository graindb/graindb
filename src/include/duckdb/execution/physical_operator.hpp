//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/execution/physical_operator.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/catalog/catalog.hpp"
#include "duckdb/common/common.hpp"
#include "duckdb/common/enums/physical_operator_type.hpp"
#include "duckdb/common/types/data_chunk.hpp"
#include "duckdb/parser/statement/select_statement.hpp"
#include "duckdb/planner/expression.hpp"
#include "duckdb/planner/joinside.hpp"
#include "duckdb/planner/logical_operator.hpp"

namespace duckdb {
class ClientContext;
class ExpressionExecutor;
class PhysicalOperator;

//! The current state/context of the operator. The PhysicalOperatorState is
//! updated using the GetChunk function, and allows the caller to repeatedly
//! call the GetChunk function and get new batches of data everytime until the
//! data source is exhausted.
class PhysicalOperatorState {
public:
	PhysicalOperatorState(PhysicalOperator *child);
	virtual ~PhysicalOperatorState() = default;

	//! Flag indicating whether or not the operator is finished [note: not all
	//! operators use this flag]
	bool finished;
	//! DataChunk that stores data from the child of this operator
	DataChunk child_chunk;
	//! State of the child of this operator
	unique_ptr<PhysicalOperatorState> child_state;
};

//! PhysicalOperator is the base class of the physical operators present in the
//! execution plan
/*!
    The execution model is a pull-based execution model. GetChunk is called on
   the root node, which causes the root node to be executed, and presumably call
   GetChunk again on its child nodes. Every node in the operator chain has a
   state that is updated as GetChunk is called: PhysicalOperatorState (different
   operators subclass this state and add different properties).
*/
class PhysicalOperator {
public:
	PhysicalOperator(PhysicalOperatorType type, vector<TypeId> types) : type(type), types(types) {
	}
	virtual ~PhysicalOperator() {
	}

	//! The physical operator type
	PhysicalOperatorType type;
	//! The set of children of the operator
	vector<unique_ptr<PhysicalOperator>> children;
	//! The types returned by this physical operator
	vector<TypeId> types;

public:
	string ToString(idx_t depth = 0) const;
	string ToJSON() const;
	void Print();

	//! Return a vector of the types that will be returned by this operator
	vector<TypeId> &GetTypes() {
		return types;
	}
	//! Initialize a given chunk to the types that will be returned by this
	//! operator, this will prepare chunk for a call to GetChunk. This method
	//! only has to be called once for any amount of calls to GetChunk.
	virtual void InitializeChunk(DataChunk &chunk) {
		auto &types = GetTypes();
		chunk.Initialize(types);
	}
	//! Retrieves a chunk from this operator and stores it in the chunk
	//! variable.
	virtual void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state,
	                              SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                              DataChunk *rai_chunk = nullptr) = 0;

	void GetChunk(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state,
	              SelectionVector *sel = nullptr, Vector *rid_vector = nullptr, DataChunk *rai_chunk = nullptr);

	virtual bool PushdownZoneFilter(idx_t table_index, const shared_ptr<bitmask_vector> &zone_filter,
	                                const shared_ptr<bitmask_vector> &zone_sel) {
		if (table_index == 0) {
			return false;
		}
		for (auto &child : children) {
			if (child->PushdownZoneFilter(table_index, zone_filter, zone_sel)) {
				return true;
			}
		}
		return false;
	}

	virtual bool PushdownRowsFilter(idx_t table_index, const shared_ptr<vector<row_t>> &rows_filter, idx_t count) {
		if (table_index == 0) {
			return false;
		}
		for (auto &child : children) {
			child->PushdownRowsFilter(table_index, rows_filter, count);
		}
		return false;
	}

	//! Create a new empty instance of the operator state
	virtual unique_ptr<PhysicalOperatorState> GetOperatorState() {
		return make_unique<PhysicalOperatorState>(children.size() == 0 ? nullptr : children[0].get());
	}

	virtual string ExtraRenderInformation() const {
		return "";
	}
};
} // namespace duckdb
