#pragma once

#include "duckdb/optimizer/optimizer.hpp"
#include "duckdb/storage/rai.hpp"

namespace duckdb {

struct RAIReference {
	RAI *rai;
	RAIType rai_type;
	LogicalComparisonJoin *op;

	RAIReference() : rai(nullptr), rai_type(RAIType::INVALID), op(nullptr) {
	}

	RAIReference(RAI *rai, RAIType rai_type, LogicalComparisonJoin *op) : rai(rai), rai_type(rai_type), op(op) {
	}
};

static inline bool SingleGet(LogicalOperator &op) {
	if (op.children.size() > 1) {
		return false;
	}
	for (auto &child : op.children) {
		if (!SingleGet(*child)) {
			return false;
		}
	}
	return true;
}

class AdaptiveRAIJoin : public LogicalOperatorVisitor {
public:
	//! Search for joins to be rewritten
	unique_ptr<LogicalOperator> Rewrite(unique_ptr<LogicalOperator> op);

	//! Override this function to search for join operators
	void VisitOperator(LogicalOperator &op) override;
};
} // namespace duckdb
