#include "duckdb/execution/physical_operator.hpp"

#include "duckdb/common/string_util.hpp"
#include "duckdb/main/client_context.hpp"

using namespace duckdb;
using namespace std;

string PhysicalOperator::ToString(idx_t depth) const {
	string extra_info = StringUtil::Replace(ExtraRenderInformation(), "\n", " ");
	StringUtil::RTrim(extra_info);
	if (!extra_info.empty()) {
		extra_info = "[" + extra_info + "]";
	}
	string result = PhysicalOperatorToString(type) + extra_info;
	if (children.size() > 0) {
		for (idx_t i = 0; i < children.size(); i++) {
			result += "\n" + string(depth * 4, ' ');
			auto &child = children[i];
			result += child->ToString(depth + 1);
		}
		result += "";
	}
	return result;
}

static string ToJSONRecursive(const PhysicalOperator &node) {
	string result = "{ \"name\": \"" + PhysicalOperatorToString(node.type) + "\",\n";
	result += "\"timing\":" + StringUtil::Format("%.2f", 0) + ",\n";
	result += "\"cardinality\":" + to_string(0) + ",\n";
	result += "\"extra_info\": \"" + StringUtil::Replace(node.ExtraRenderInformation(), "\n", " ") + "\",\n";
	result += "\"children\": [";
	result += StringUtil::Join(node.children, node.children.size(), ",\n",
	                           [](const unique_ptr<PhysicalOperator> &child) { return ToJSONRecursive(*child); });
	result += "]\n}\n";
	return result;
}

string PhysicalOperator::ToJSON() const {
	string result = "{ \"result\": " + to_string(0.1) + ",\n";
	// print the phase timings
	result += "\"timings\": {},\n";
	// recursively print the physical operator tree
	result += "\"tree\": ";
	result += ToJSONRecursive(*this);

	return result + "}";
}

PhysicalOperatorState::PhysicalOperatorState(PhysicalOperator *child) : finished(false) {
	if (child) {
		child->InitializeChunk(child_chunk);
		child_state = child->GetOperatorState();
	}
}

void PhysicalOperator::GetChunk(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state,
                                SelectionVector *sel, Vector *rid_vector, DataChunk *rai_chunk) {
	if (context.interrupted) {
		throw InterruptException();
	}

	chunk.Reset();
	if (state->finished) {
		return;
	}

	context.profiler.StartOperator(this);
	GetChunkInternal(context, chunk, state, sel, rid_vector, rai_chunk);
	context.profiler.EndOperator(chunk);

	chunk.Verify();
}

void PhysicalOperator::Print() {
	Printer::Print(ToString());
}
