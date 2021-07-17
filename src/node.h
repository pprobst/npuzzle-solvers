#pragma once
#ifndef NODE_H
#define NODE_H

#include "state.h"
#include "heuristic.h"
#include <vector>
#include <memory>

/**
 * Node
 *
 * A node is a data structure constituting part of a search tree.
 * Includes state, parent node, action, path cost g, heuristic h.
 */


class Node {
    private:
    static uint64_t node_counter;

    public:
    State state;
    State* parent_state;
	Action action;
    std::size_t idx;
	uint32_t cost; // g
	int heuristic; // h

	Node();
	Node(State* parent_state, Action a, const State& s, uint32_t cost, Heuristic& h);
	Node(State* parent_state, Action a, const State& s, uint32_t cost);

	void makeRootNode(const State& s, Heuristic& h);
	void makeRootNode(const State& s);

    inline Node& operator=(const Node &n) {
        this->state = n.state;
        this->parent_state = n.parent_state;
        this->action = n.action;
        this->idx = n.idx;
        this->cost = n.cost;
        this->heuristic = n.heuristic;
        return *this;
    }
};

#endif
