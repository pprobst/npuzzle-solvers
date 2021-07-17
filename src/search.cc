#include "search.h"
#include <algorithm>

using namespace std;

/*
vector<Action> Search::extractPath(Node node) {
    vector<Action> solution_path;

    while (node.parent != nullptr || node.state->action != Action::NONE) {
        solution_path.push_back(node.state->action);
        node = *node.parent;
    }

    return solution_path;
}
*/

Search::Solution Search::makeSolution(uint32_t solution_length) {
    return Solution{
        this->expanded_node_count,
        solution_length, // optimal solution length
        std::chrono::duration_cast<std::chrono::nanoseconds>(this->end_clock -
                                                             this->start_clock)
                .count() /
            1000000000.0,
        this->heuristic.accumulator == 0
            ? 0
            : double(this->heuristic.accumulator) /
                  this->heuristic.called_count, // heuristic mean value
        this->start.heuristic, // initial state's heuristic value
    };
}
