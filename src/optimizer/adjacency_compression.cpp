#include "duckdb/optimizer/adjacency_compression.hpp"

#include "duckdb/planner/expression/bound_columnref_expression.hpp"

using namespace duckdb;
using namespace std;

// !FIXME: merge join conditions
unique_ptr<LogicalOperator> AdjacencyCompression::Optimize(unique_ptr<LogicalOperator> op) {
	if (op->type == LogicalOperatorType::COMPARISON_JOIN) {
		auto n_op = Compress(move(op));
		auto n_op_ptr = (LogicalComparisonJoin *)n_op.get();
		if (n_op_ptr->left_is_edge) {
			n_op_ptr->conditions[0].ReplaceLeft(move(n_op_ptr->source_expr));
		} else {
			n_op_ptr->conditions[0].ReplaceRight(move(n_op_ptr->source_expr));
		}
		return n_op;
	}
	for (auto &child : op->children) {
		child = Optimize(move(child));
	}
	return op;
}

unique_ptr<LogicalOperator> AdjacencyCompression::Compress(unique_ptr<LogicalOperator> op) {
	auto comp_join = reinterpret_cast<LogicalComparisonJoin *>(op.get());
	if (comp_join->join_type != JoinType::ADJACENCY) {
		return op;
	} else {
		if (comp_join->left_is_edge) {
			auto left_edge = comp_join->children[0].get();
			switch (left_edge->type) {
			case LogicalOperatorType::GET: {
				auto get_op = reinterpret_cast<LogicalGet *>(left_edge);
				comp_join->edges.push_back(get_op->table);
				comp_join->children[0] = nullptr;
				if (comp_join->source_expr == nullptr) {
					comp_join->source_expr = move(comp_join->conditions[0].right);
				}
				break;
			}
			case LogicalOperatorType::COMPARISON_JOIN: {
				auto n_op = Compress(move(comp_join->children[0]));
				auto n_op_ptr = (LogicalComparisonJoin *)n_op.get();
				comp_join->source_expr = move(n_op_ptr->source_expr);
				auto n_edge_op = reinterpret_cast<LogicalComparisonJoin *>(n_op.get());
				if (n_edge_op->join_type == JoinType::ADJACENCY) {
					comp_join->edges.insert(comp_join->edges.end(), n_edge_op->edges.begin(), n_edge_op->edges.end());
					if (n_op->children[0] != nullptr && n_op->children[1] != nullptr) {
						comp_join->children[0] = move(n_op->children[n_edge_op->left_is_edge ? 0 : 1]);
					} else {
						comp_join->children[0] =
						    move(n_op->children[1] == nullptr ? n_op->children[0] : n_op->children[1]);
					}
				} else {
					comp_join->children[0] = move(n_op);
				}
				break;
			}
			default:
				throw Exception("Logical op type not supported yet in adjacency compression");
			}
		}
		if (comp_join->right_is_edge) {
			auto right_edge = comp_join->children[1].get();
			switch (right_edge->type) {
			case LogicalOperatorType::GET: {
				auto get_op = reinterpret_cast<LogicalGet *>(right_edge);
				comp_join->edges.push_back(get_op->table);
				comp_join->children[1] = nullptr;
				if (comp_join->source_expr == nullptr) {
					comp_join->source_expr = move(comp_join->conditions[0].left);
				}
				break;
			}
			case LogicalOperatorType::COMPARISON_JOIN: {
				auto n_op = Compress(move(comp_join->children[1]));
				auto n_op_ptr = (LogicalComparisonJoin *)n_op.get();
				comp_join->source_expr = move(n_op_ptr->source_expr);
				auto n_edge_op = reinterpret_cast<LogicalComparisonJoin *>(n_op.get());
				if (n_edge_op->join_type == JoinType::ADJACENCY) {
					comp_join->edges.insert(comp_join->edges.end(), n_edge_op->edges.begin(), n_edge_op->edges.end());
					if (n_op->children[0] != nullptr && n_op->children[1] != nullptr) {
						comp_join->children[1] = move(n_op->children[n_edge_op->left_is_edge ? 0 : 1]);
					} else {
						comp_join->children[1] =
						    move(n_op->children[0] == nullptr ? n_op->children[1] : n_op->children[0]);
					}
				} else {
					comp_join->children[1] = move(n_op);
				}
				break;
			}
			default:
				throw Exception("Logical op type not supported yet in adjacency compression");
			}
		}
		return op;
	}
}
