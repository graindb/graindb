#include "duckdb/planner/operator/logical_comparison_join.hpp"

#include "duckdb/common/string_util.hpp"

using namespace duckdb;
using namespace std;

LogicalComparisonJoin::LogicalComparisonJoin(JoinType join_type, LogicalOperatorType logical_type)
    : LogicalJoin(join_type, logical_type), enable_lookup_join(false) {
}

string LogicalComparisonJoin::ParamsToString() const {
	string result = "[" + JoinTypeToString(join_type);
	if (conditions.size() > 0) {
		result += " ";
		result += StringUtil::Join(conditions, conditions.size(), ", ", [](const JoinCondition &condition) {
			return ExpressionTypeToString(condition.comparison) + "(" + condition.left->GetName() + ", " +
			       condition.right->GetName() + ")";
		});
		result += ", ";
		switch (op_mark) {
		case OpMark::HASH_JOIN:
			result += "HASH JOIN";
			break;
		case OpMark::SIP_JOIN:
			result += "RAI JOIN";
			break;
		case OpMark::NLJ:
			result += "NEST LOOP JOIN";
			break;
		case OpMark::MERGED_SIP_JOIN:
			result += "MERGED_RAI_JOIN";
			break;
		default:
			result += "DEFAULT JOIN";
			break;
		}
		result += "]";
	}

	return result;
}
