#include "duckdb/execution/operator/filter/physical_filter.hpp"

#include "duckdb/execution/expression_executor.hpp"
#include "duckdb/planner/expression/bound_conjunction_expression.hpp"

using namespace duckdb;
using namespace std;

class PhysicalFilterState : public PhysicalOperatorState {
public:
	PhysicalFilterState(PhysicalOperator *child, Expression &expr) : PhysicalOperatorState(child), executor(expr) {
	}

	ExpressionExecutor executor;
};

PhysicalFilter::PhysicalFilter(vector<TypeId> types, vector<unique_ptr<Expression>> select_list)
    : PhysicalOperator(PhysicalOperatorType::FILTER, types) {
	assert(select_list.size() > 0);
	if (select_list.size() > 1) {
		// create a big AND out of the expressions
		auto conjunction = make_unique<BoundConjunctionExpression>(ExpressionType::CONJUNCTION_AND);
		for (auto &expr : select_list) {
			conjunction->children.push_back(move(expr));
		}
		expression = move(conjunction);
	} else {
		expression = move(select_list[0]);
	}
}

void PhysicalFilter::GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
                                      SelectionVector *sel, Vector *rid_vector, DataChunk *rai_chunk) {
	auto state = reinterpret_cast<PhysicalFilterState *>(state_);
	SelectionVector filter_sel(STANDARD_VECTOR_SIZE);
	idx_t initial_count;
	idx_t result_count;
	do {
		// fetch a chunk from the child and run the filter
		// we repeat this process until either (1) passing tuples are found, or (2) the child is completely exhausted
		children[0]->GetChunk(context, chunk, state->child_state.get(), sel, rid_vector, rai_chunk);
		if (chunk.size() == 0) {
			return;
		}
		initial_count = chunk.size();
		result_count = state->executor.SelectExpression(chunk, filter_sel);
	} while (result_count == 0 && rid_vector == nullptr);

	if (result_count == initial_count) {
		// nothing was filtered: skip adding any selection vectors
		return;
	}
	chunk.Slice(filter_sel, result_count);
	if (sel != nullptr) {
		auto sel_data = sel->Slice(filter_sel, result_count);
		sel->Initialize(move(sel_data));
	}
}

unique_ptr<PhysicalOperatorState> PhysicalFilter::GetOperatorState() {
	return make_unique<PhysicalFilterState>(children[0].get(), *expression);
}

string PhysicalFilter::ExtraRenderInformation() const {
	return expression->GetName();
}
