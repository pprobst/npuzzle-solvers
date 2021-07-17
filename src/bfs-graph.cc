#include "bfs-graph.h"
#include <deque>
#include <unordered_set>

using namespace std;

BFSGraph::BFSGraph() {}

/*
 * Breadth-First Search Graph (duplicate elim).
 *
 * While the open set (deque, a double-ended queue) is not empty, expand a node.
 * Then generate the successors for the current node; if one of
 * successor states is the goal, end the search; else, if the state
 * is not in the closed set, add the state to it and the newly
 * generated node to the end of the deque.
 * The deque is efficient for insertion/deletion at the end/begin.
 *
 * The closed list remembers expanded states to avoid duplicated expansions of
 * the same state. The open list (frontier) determines the next node to expand.
 */
optional<Search::Solution> BFSGraph::run(State initial_state) {
    this->start_clock = chrono::steady_clock::now();

    if (initial_state.isGoal()) {
        this->end_clock = chrono::steady_clock::now();
        return this->makeSolution(0);
    }

    deque<Node> open;
    this->start.makeRootNode(initial_state);
    open.push_back(this->start);

    unordered_set<uint64_t> closed;
    closed.insert(this->start.state.id);

    while (!open.empty()) {
        Node n = open.front();
        open.pop_front();

        this->expanded_node_count++;
        for (State &s_ : n.state.succ()) {
            Node n_ = Node(&n.state, s_.action, s_, n.cost);
            if (s_.isGoal()) {
                this->end_clock = chrono::steady_clock::now();
                return this->makeSolution(n_.cost);
            } else if (closed.find(s_.id) == closed.end()) {
                closed.insert(s_.id);
                open.push_back(n_);
            }
        }
    }

    return nullopt;
}
