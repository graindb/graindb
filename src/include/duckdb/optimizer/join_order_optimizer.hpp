//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/optimizer/join_order_optimizer.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/constants.hpp"
#include "duckdb/common/unordered_map.hpp"
#include "duckdb/common/unordered_set.hpp"
#include "duckdb/optimizer/join_order/join_relation.hpp"
#include "duckdb/optimizer/join_order/query_graph.hpp"
#include "duckdb/parser/expression_map.hpp"
#include "duckdb/planner/logical_operator.hpp"
#include "duckdb/planner/logical_operator_visitor.hpp"
#include "duckdb/planner/operator/logical_get.hpp"
#include "nlohmann_json/single_include/nlohmann/json.hpp"

#include <functional>
#include <iostream>

//#define LOG_ENUMERATION
//#define PRINT_JOIN_NODE

namespace duckdb {
using json = nlohmann::json;

struct JoinOrderNode {
	OpMark op_mark;
	vector<unique_ptr<JoinOrderNode>> children;
	idx_t relation_id;
};

class JoinOrderOptimizer {
public:
	//! Represents a node in the join plan
	struct JoinNode {
		JoinRelationSet *set;
		NeighborInfo *info;
		idx_t cardinality;
		double cost;
		JoinNode *left;
		JoinNode *right;
		OpMark op_mark;

		//! Create a leaf node in the join tree
		JoinNode(JoinRelationSet *set, idx_t cardinality)
		    : set(set), info(nullptr), cardinality(cardinality), cost(cardinality), left(nullptr), right(nullptr),
		      op_mark(OpMark::SCAN) {
#ifdef LOG_ENUMERATION
			cout << "Base set " + set->ToString() + ", Cardinality:" + to_string(cardinality) +
			            ", Cost: " + to_string(cost)
			     << endl;
#endif
		}
		//! Create an intermediate node in the join tree
		JoinNode(JoinRelationSet *set, NeighborInfo *info, JoinNode *left, JoinNode *right, idx_t cardinality,
		         double cost, OpMark op_mark = OpMark::JOIN)
		    : set(set), info(info), cardinality(cardinality), cost(cost), left(left), right(right), op_mark(op_mark) {
		}

		static string OpMarkToString(OpMark mark) {
			switch (mark) {
			case OpMark::NLJ:
			case OpMark::HASH_JOIN:
			case OpMark::SIP_JOIN:
			case OpMark::JOIN: {
				return "JOIN";
			}
			case OpMark::MERGED_SIP_JOIN: {
			}
				return "MERGE_JOIN";
			case OpMark::SCAN: {
				return "SCAN";
			}
			default:
				return "NO_OP";
			}
		}

		static string ToJSONRecursive(const JoinNode &node) {
			string result = "{ \"name\": \"" + OpMarkToString(node.op_mark) + "\",\n";
			result += "\"timing\": 0.0,\n";
			result += "\"cardinality\":" + to_string(node.cardinality) + ",\n";
			result += "\"extra_info\": \"[cost: " + to_string(node.cost) + "]\",\n";
			result += "\"children\": [";
			if (node.left) {
				result += ToJSONRecursive(*node.left);
				result += ",\n";
			}
			if (node.right) {
				result += ToJSONRecursive(*node.right);
			}
			result += "]\n}\n";
			return result;
		}

		string ToJSON() {
			string result = "{ \"result\": " + to_string(0.1) + ",\n";
			// print the phase timings
			result += "\"timings\": {},\n";
			// recursively print the physical operator tree
			result += "\"tree\": ";
			result += ToJSONRecursive(*this);

			return result + "}";
		}
	};

public:
	JoinOrderOptimizer(ClientContext &context) : context(context) {
	}
	//! Perform join reordering inside a plan
	unique_ptr<LogicalOperator> Optimize(unique_ptr<LogicalOperator> plan);

private:
	ClientContext &context;
	//! The total amount of join pairs that have been considered
	idx_t pairs = 0;
	//! Set of all relations considered in the join optimizer
	vector<unique_ptr<SingleJoinRelation>> relations;
	//! A mapping of base table index -> index into relations array (relation number)
	unordered_map<idx_t, idx_t> relation_mapping;
	//! A mapping of base table table_alias -> index into relations array (relation number)
	unordered_map<string, idx_t> relation_alias_mapping;
	//! A structure holding all the created JoinRelationSet objects
	JoinRelationSetManager set_manager;
	//! The set of edges used in the join optimizer
	QueryGraph query_graph;
	//! The optimal join plan found for the specific JoinRelationSet*
	unordered_map<JoinRelationSet *, unique_ptr<JoinNode>> plans;
	//! The set of filters extracted from the query graph
	vector<unique_ptr<Expression>> filters;
	//! The set of filter infos created from the extracted filters
	vector<unique_ptr<FilterInfo>> filter_infos;
	//! A map of all expressions a given expression has to be equivalent to. This is used to add "implied join edges".
	//! i.e. in the join A=B AND B=C, the equivalence set of {B} is {A, C}, thus we can add an implied join edge {A <->
	//! C}
	expression_map_t<vector<FilterInfo *>> equivalence_sets;

	unordered_map<JoinRelationSet *, idx_t> cached_cardinalities;

	//! Extract the bindings referred to by an Expression
	bool ExtractBindings(Expression &expression, unordered_set<idx_t> &bindings);
	//! Traverse the query tree to find (1) base relations, (2) existing join conditions and (3) filters that can be
	//! rewritten into joins. Returns true if there are joins in the tree that can be reordered, false otherwise.
	bool ExtractJoinRelations(LogicalOperator &input_op, vector<LogicalOperator *> &filter_operators,
	                          LogicalOperator *parent = nullptr);
	//! Emit a pair as a potential join candidate. Returns the best plan found for the (left, right) connection (either
	//! the newly created plan, or an existing plan)
	JoinNode *EmitPair(JoinRelationSet *left, JoinRelationSet *right, NeighborInfo *info);
	//! Tries to emit a potential join candidate pair. Returns false if too many pairs have already been emitted,
	//! cancelling the dynamic programming step.
	bool TryEmitPair(JoinRelationSet *left, JoinRelationSet *right, NeighborInfo *info);

	bool EnumerateCmpRecursive(JoinRelationSet *left, JoinRelationSet *right, unordered_set<idx_t> exclusion_set);
	//! Emit a relation set node
	bool EmitCSG(JoinRelationSet *node);
	//! Enumerate the possible connected subgraphs that can be joined together in the join graph
	bool EnumerateCSGRecursive(JoinRelationSet *node, unordered_set<idx_t> &exclusion_set);
	//! Rewrite a logical query plan given the join plan
	unique_ptr<LogicalOperator> RewritePlan(unique_ptr<LogicalOperator> plan, JoinNode *node);
	//! Generate cross product edges inside the side
	void GenerateCrossProducts();
	//! Perform the join order solving
	void SolveJoinOrder();
	//! Parse injected join order json string into join_order_tree
	unique_ptr<JoinOrderNode> ParseJONode(nlohmann::json &jo_node);
	//! Inject join order
	JoinRelationSet *InjectJoinNode(unique_ptr<JoinOrderNode> join_order);
	//! Inject cardinality
	void InjectCardinality(nlohmann::json &jo);
	//! Solve the join order exactly using dynamic programming. Returns true if it was completed successfully (i.e. did
	//! not time-out)
	bool SolveJoinOrderExactly();
	//! Solve the join order approximately using a greedy algorithm
	void SolveJoinOrderApproximately();
	unique_ptr<JoinNode> CreateJoinTree(JoinRelationSet *set, NeighborInfo *info, JoinNode *left, JoinNode *right);

	unique_ptr<LogicalOperator> ResolveJoinConditions(unique_ptr<LogicalOperator> op);
	std::pair<JoinRelationSet *, unique_ptr<LogicalOperator>>
	GenerateJoins(vector<unique_ptr<LogicalOperator>> &extracted_relations, JoinNode *node);
};

} // namespace duckdb
