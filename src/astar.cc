#include "astar.h"
#include <unordered_set>
#include <unordered_map>
#include <queue>

using namespace std;

AStar::AStar(Heuristic h) {
    this->heuristic = h;
}

// Comparator used by the priority queue in A*.
// priority: f -> h -> LIFO
struct AStarComparator {
	bool operator()(Node& a, Node& b) {
		int f_a = a.cost + a.heuristic;
		int f_b = b.cost + b.heuristic;
		// f
		if (f_a != f_b) {
			return f_a > f_b;
		}
		// h
		else if (a.heuristic != b.heuristic) {
			return a.heuristic > b.heuristic;
		}
		// LIFO (idx)
		return a.idx < b.idx;
	}
};

/* A* without reopening, as we're using an admissable and optimal heuristic.
 *
 * While the open list (a min-heap ordered by f, h, idx; gets the lowest value) is not empty, 
 * expand a node.
 * If the state of the curr node is not in the closed set, add the state to it, and generate 
 * the successors of the current node and add them to the open list.
 */
optional<Search::Solution> AStar::run(State initial_state) {
    this->start_clock = chrono::steady_clock::now();

	if (initial_state.isGoal()) {
        this->end_clock = chrono::steady_clock::now();
		return this->makeSolution(0);
	}

	priority_queue<Node, vector<Node>, AStarComparator> open;
    this->start.makeRootNode(initial_state, this->heuristic);
	open.push(this->start);

	unordered_set<uint64_t> closed;

	while (!open.empty()) {
		Node n = open.top();
		open.pop();

		if (closed.find(n.state.id) == closed.end()) {
			closed.insert(n.state.id);
			if (n.state.isGoal()) {
                this->end_clock = chrono::steady_clock::now();
				return this->makeSolution(n.cost);
			}
            this->expanded_node_count++;
			for (State& s : n.state.succ()) {
				Node n_ = Node(&n.state, s.action, s, n.cost, this->heuristic);
				open.push(n_);
			}
		}
	}

	return nullopt;
}
