#pragma once

#include "duckdb/optimizer/optimizer.hpp"

namespace duckdb {

class SIPJoinRewriter : public LogicalOperatorVisitor {

public:
	//! Search for joins to be rewritten
	unique_ptr<LogicalOperator> Rewrite(unique_ptr<LogicalOperator> op);

	//! Override this function to search for join operators
	void VisitOperator(LogicalOperator &op) override;

private:
	unordered_map<idx_t, vector<RAIInfo *>> rai_info_map;

	void DoRewrite(LogicalComparisonJoin &join);
	bool BindRAIInfo(LogicalComparisonJoin &join, vector<unique_ptr<RAI>> &rais, JoinCondition &condition);
};
} // namespace duckdb
