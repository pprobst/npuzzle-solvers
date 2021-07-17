#include "state.h"
#include <iostream>
//#include <bitset>

using namespace std;

/*
 * Each tile in a state takes 4 bits, so the whole state has, at most, the size
 * of 64 bits in the 15 puzzle, and 36 bits in the 8 puzzle. Because of this,
 * the state will be stored in an uint64_t (8 bytes).
 *
 * <<   shift left
 * >>   shift right
 * |    bitwise or
 * &    bitwise and
 * ~    bitwise negation
 */

State::State() {}

State::State(vector<uint8_t> &problem, Action action) {
    this->tiles = problem;
    this->action = action;

    this->id = 0;
    for (int i = problem.size() - 1; i >= 0; i--) {
        if (problem[i] == 0)
            this->blank_idx = i;
        this->id <<= 4;
        this->id |= problem[i];
    }

    this->size = problem.size();
    this->dim = sqrt(this->size);

    if (this->size == 9) {
        this->goal_id = 36344967696; // 0x876543210 -> initial state
    } else if (this->size == 16) {
        this->goal_id =
            18364758544493064720U; // 0xfedcba9876543210 -> goal state
    }
}

State::State(uint64_t state_id, Action action, uint8_t blank_idx, uint8_t size,
             uint8_t dim, uint64_t goal_id) {
    this->action = action;
    this->blank_idx = blank_idx;
    this->size = size;
    this->dim = dim;
    this->id = state_id;
    this->goal_id = goal_id;
}

State State::makeInitialState(vector<uint8_t> &problem) {
    return State(problem, Action::NONE);
}

vector<State> State::succ(State *parent) const {
    vector<State> succ_states;
    Action action;
    State state;

    for (int actionInt = UP; actionInt != NONE; actionInt++) {
        action = static_cast<Action>(actionInt);
        state = this->getStateFrom(action);
        if (state != *this) {
            if (parent == nullptr || state != *parent)
                succ_states.push_back(state);
        }
    }

    return succ_states;
}

State State::getStateFrom(Action action) const {
    pair<uint8_t, uint8_t> curr_blank_pos = this->idxPos(this->blank_idx);

    if (!this->validAction(action, curr_blank_pos)) {
        return *this;
    }

    pair<uint8_t, uint8_t> new_blank_pos =
        this->idxPos(this->blank_idx) + MOVES.at(action);
    size_t new_blank_idx = this->posIdx(new_blank_pos);

    // Example:
    //
    // state: 0x518630742
    //  0     0    0   0    0    0     0    5   1    8    6    3    0    7    4
    //  2
    // 0000 0000 0000 0000 0000 0000 0000 0101 0001 1000 0110 0011 0000 0111
    // 0100 0010
    //
    // Action::RIGHT (0 swaps with 3)
    //
    // value_to_swap: 0x30000 (3)
    // 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0011 0000 0000
    // 0000 0000
    //
    // blank_swap_state: 0x518600742
    // 0000 0000 0000 0000 0000 0000 0000 0101 0001 1000 0110 0000 0000 0111
    // 0100 0010
    //
    // new_blank_idx > current_blank_idx, so:
    // value_to_swap >>= 4 * (new_blank_idx - this->blank_idx);
    // 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0011 0000
    // 0000 0000
    //
    // new_state_id = blank_swap_state OR value_to_swap
    // 0000 0000 0000 0000 0000 0000 0000 0101 0001 1000 0110 0000 0000 0111
    // 0100 0010
    //                                    OR
    // 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0011 0000
    // 0000 0000
    // -------------------------------------------------------------------------------
    // 0000 0000 0000 0000 0000 0000 0000 0101 0001 1000 0110 0000 0011 0111
    // 0100 0010

    uint64_t mask = 0xf;
    mask <<= 4 * new_blank_idx;
    uint64_t value_to_swap = this->id & mask;
    uint64_t blank_swap_state = this->id & ~mask;
    uint8_t moves = 4 * abs(int(new_blank_idx - this->blank_idx));

    if (new_blank_idx > this->blank_idx)
        value_to_swap >>= moves;
    else
        value_to_swap <<= moves;

    uint64_t new_state_id = blank_swap_state | value_to_swap;

    return State(new_state_id, action, new_blank_idx, this->size, this->dim,
                 this->goal_id);

    /* PREVIOUS VECTOR-BASED SOLUTION.
    vector<int> new_state_tiles;
    new_state_tiles.reserve(this->size);
    new_state_tiles = this->tiles;
    pair<int, int> new_blank_pos = curr_blank_pos + MOVES.at(action);

    int new_blank_idx = this->posIdx(new_blank_pos);
    int value_to_swap = this->tiles[new_blank_idx];

    new_state_tiles[new_blank_idx] = 0;
    new_state_tiles[this->blank_idx] = value_to_swap;

    return State(new_state_tiles, action, new_blank_idx, this->size, this->dim,
                 this->goal_id);
    */
}

bool State::validAction(Action action, pair<uint8_t, uint8_t> pos) const {
    switch (action) {
    case Action::DOWN:
        if (pos.second == this->dim - 1)
            return false;
        break;
    case Action::LEFT:
        if (pos.first == 0)
            return false;
        break;
    case Action::RIGHT:
        if (pos.first == this->dim - 1)
            return false;
        break;
    case Action::UP:
        if (pos.second == 0)
            return false;
        break;
    case Action::NONE:
        return false;
    }
    return true;
}
