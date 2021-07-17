#include "ids.h"
#include <limits>

using namespace std;

IDS::IDS() {}

/*
 * Depth-Limited Search
 *
 * Changes to the "common" algorithm:
 *
 * - Using a boolean return instead of a deque, as we don't need to retrieve the
 * solution path, which has size == depth reached. However, even without
 * push_fronts, there was no substantial performance gain (push_front is very
 * well-optimized).
 *
 * - DLS takes as input a Node, and not a State -- this is because we can more
 * easily check its parent node's state (we don't want to generate successor
 * states that are equal to the current node's parent state).
 */
bool IDS::DLS(Node &node, int depth_limit) {
    if (node.state.isGoal()) {
        return true;
    }

    if (depth_limit > 0) {
        this->expanded_node_count++;

        vector<State> successors;
        if (node.parent_state != nullptr)
            successors = node.state.succ(node.parent_state);
        else
            successors = node.state.succ();

        for (State &s : successors) {
            Node n_ = Node(&node.state, s.action, s, node.cost);
            bool solution = this->DLS(n_, depth_limit - 1);
            if (solution) {
                return true;
            }
        }
    }

    return false;
}

/*
 * Iterative Deepening Search
 *
 * The idea is to perform a sequence of depth-limited searches with increasing
 * depth limit. Each iteration "repeats the work" of all previous iterations, so
 * it may be inneficient.
 */
optional<Search::Solution> IDS::run(State initial_state) {
    this->start_clock = chrono::steady_clock::now();
    this->start.makeRootNode(initial_state);

    for (int depth_limit = 0; depth_limit < numeric_limits<int>::max();
         depth_limit++) {
        bool solution = this->DLS(this->start, depth_limit);
        if (solution) {
            this->end_clock = chrono::steady_clock::now();
            return this->makeSolution(depth_limit);
        }
    }

    return nullopt;
}
