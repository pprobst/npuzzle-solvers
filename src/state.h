#pragma once
#ifndef STATE_H
#define STATE_H

//#include "node.h"
#include <vector>
#include <map>
#include <utility>
#include <cmath>
#include <iostream>
#include <memory>

/**
 * State
 *
 * A state is a (representation of a) physical configuration.
 * In this case, the state is the N-Puzzle configuration.
 * States do not have parents, children, depth, cost, etc.
 *
 *   8-puzzle
 *   --------
 *   0 1 2
 *   3 4 5
 *   6 7 8
 *
 *   15-puzzle
 *   ---------
 *   0   1   2   3
 *   4   5   6   7
 *   8   9   10  11
 *   12  13  14  15
 *
 */


enum Action {
    UP,
    LEFT,
    RIGHT,
    DOWN,
    NONE,
};

class State {
	public:

    std::vector<uint8_t> tiles;
    uint8_t blank_idx; // index of the blank tile [0,8) or [0,16)
    uint8_t size; // puzzle size (9 or 16)
    uint8_t dim; // dimensions (3 or 4)
    uint64_t id; // internal representation of this state
    uint64_t goal_id; // internal representation of the goal state
	Action action;

    State();
    State(std::vector<uint8_t>& problem, Action a);
    State(uint64_t state_id, Action a, uint8_t blank_idx, uint8_t size,
          uint8_t dim, uint64_t goal_id);

    static State makeInitialState(std::vector<uint8_t>& problem);

    std::vector<State> succ(State* parent = nullptr) const;
    State getStateFrom(Action a) const;
    bool validAction(Action action, std::pair<uint8_t, uint8_t> pos) const;

    inline std::pair<uint8_t, uint8_t> idxPos(uint8_t idx) const {
      return {idx % this->dim, idx / this->dim};
    }

    inline int posIdx(std::pair<uint8_t, uint8_t> pos) const {
        return (pos.second*this->dim) + pos.first;
    }

    inline uint8_t getTileAt(uint8_t idx) const {
        uint64_t mask = 0xf; // 0000 ... 0000 1111 --> takes the idx of the first tile (but last internally).
        mask <<= 4*idx; // shift left to the index position needed.
        uint8_t tile_at_idx = (this->id & mask) >> 4*idx;
        return tile_at_idx;
    }

    inline bool isGoal() const {
        return this->id == this->goal_id;
    }

    // Operator overrides
    inline bool operator==(const State &rhs) const {
        return (this->id == rhs.id);
    }
    inline bool operator!=(const State &rhs) const {
        return !this->operator==(rhs);
    }

};


namespace std {

const std::map<Action, std::pair<uint8_t, uint8_t>> MOVES = {
    { Action::UP,    {0, -1} },
    { Action::LEFT,  {-1, 0} },
    { Action::RIGHT, {1, 0} },
    { Action::DOWN,  {0, 1} },
};

inline std::pair<uint8_t, uint8_t> operator+(const std::pair<uint8_t, uint8_t> &lhs, const std::pair<uint8_t, uint8_t> &rhs) {
    return {lhs.first + rhs.first, lhs.second + rhs.second};
}

};

#endif
