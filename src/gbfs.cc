#include "gbfs.h"
#include "node.h"
#include <limits>
#include <queue>
#include <unordered_set>
#include <iostream>

using namespace std;

GBFS::GBFS(Heuristic h) {
    this->heuristic = h;
}

// Comparator used by the priority queue in GBFS.
// priority: h -> g -> LIFO
struct GBFSComparator {
	bool operator()(Node& a, Node& b) {
		// h
		if (a.heuristic != b.heuristic) {
			return a.heuristic > b.heuristic;
		}
		// g
		else if (a.cost != b.cost) {
			return a.cost < b.cost;
		}
		// LIFO (idx)
		return a.idx < b.idx;
	}
};

/* Greedy Best-First Search
 * 
 * Best-First Search algorithms expand the "most promising" nodes according to some heuristic.
 * GBFS is the same as A*, but with a different comparator (does not consider g+h, or F).
 */
optional<Search::Solution> GBFS::run(State initial_state) {
    this->start_clock = chrono::steady_clock::now();

	if (initial_state.isGoal()) {
        this->end_clock = chrono::steady_clock::now();
		return this->makeSolution(0);
	}

    this->start.makeRootNode(initial_state, this->heuristic);
	priority_queue<Node, vector<Node>, GBFSComparator> open;
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
