#pragma once

#include "duckdb/optimizer/optimizer.hpp"

namespace duckdb {

class SIPJoinMerger : public LogicalOperatorVisitor {
public:
	//! Search for joins to be rewritten
	unique_ptr<LogicalOperator> Rewrite(unique_ptr<LogicalOperator> op);

	//! Override this function to search for join operators
	void VisitOperator(LogicalOperator &op) override;

private:
	void Merge(LogicalComparisonJoin &join);
};
}
