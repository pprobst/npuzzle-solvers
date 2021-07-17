#include "idastar.h"
#include <limits>
#include <utility>

using namespace std;

IDAStar::IDAStar(Heuristic h) { this->heuristic = h; }

/* IDA*
 *
 * Implements ideas form iterative deepening to A* -- instead of `depth`, we
 * bound `f`. f_limit is the next useful f-bound for the subtree considered by
 * the call.
 *
 * Has low space complexity but the repeated work (like IDS) can be expensive.
 *
 * It's a tree search, i.e., duplicates are possible (no closed set). So it's
 * more efficient when there are few duplicates.
 */
pair<optional<int>, optional<int>> IDAStar::RecursiveSearch(Node &node,
                                                            int f_limit) {
    int f = node.cost + node.heuristic;
    if (f > f_limit) {
        // return <f(n), none>
        return pair<optional<int>, optional<int>>{f, nullopt};
    }
    if (node.state.isGoal()) {
        return pair<optional<int>, optional<int>>{nullopt, node.cost};
    }

    int next_limit = numeric_limits<int>::max();

    this->expanded_node_count++;

    vector<State> successors;
    if (node.parent_state != nullptr)
        successors = node.state.succ(node.parent_state);
    else
        successors = node.state.succ();

    for (State &s : successors) {
        Node n_ = Node(&node.state, s.action, s, node.cost, this->heuristic);

        pair<optional<int>, optional<int>> rs =
            this->RecursiveSearch(n_, f_limit);

        if (rs.second) {
            // return <none, solution>
            return pair<optional<int>, optional<int>>{nullopt, rs.second};
        }
        next_limit = min<int>(next_limit, rs.first.value());
    }

    // return <next_limit, none>
    return pair<optional<int>, optional<int>>{next_limit, nullopt};
}

optional<Search::Solution> IDAStar::run(State initial_state) {
    this->start_clock = chrono::steady_clock::now();

    if (initial_state.isGoal()) {
        this->end_clock = chrono::steady_clock::now();
        return this->makeSolution(vector<Action>().size());
    }

    this->start.makeRootNode(initial_state, this->heuristic);
    int f_limit = this->start.heuristic;

    while (f_limit < numeric_limits<int>::max()) {
        pair<optional<int>, optional<int>> solution =
            this->RecursiveSearch(this->start, f_limit);
        if (solution.second) {
            this->end_clock = chrono::steady_clock::now();
            return this->makeSolution(solution.second.value());
        }
        if (solution.first) {
            f_limit = solution.first.value();
        }
    }

    return nullopt;
}
