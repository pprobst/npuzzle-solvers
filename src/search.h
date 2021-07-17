#pragma once
#ifndef SEARCH_H
#define SEARCH_H

#include "state.h"
#include "node.h"
#include "heuristic.h"
#include <optional>
#include <vector>
#include <deque>
#include <utility>
#include <chrono>

class Search {
    protected:
    // Why steady_clock? tl;dr it's like a stop watch. See:
    // https://youtu.be/P32hvk8b13M?t=2875
    std::chrono::steady_clock::time_point start_clock;
    std::chrono::steady_clock::time_point end_clock;
    
    public:
    uint32_t expanded_node_count = 0;

	struct Solution {
        uint32_t expanded_nodes;
        uint32_t optimal_solution_length;
        double solution_time;
        double heuristic_mean_value;
		int heuristic_start_state;
	};

	Node start = Node();
    Heuristic heuristic;

	Solution makeSolution(uint32_t solution_length);
    virtual std::optional<Solution> run(State state) = 0;
};

#endif
