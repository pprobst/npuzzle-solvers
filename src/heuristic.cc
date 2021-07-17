#include "heuristic.h"

using namespace std;

Heuristic::Heuristic() {
    this->called_count = 0;
    this->accumulator = 0;
}

int Heuristic::calculate(const State &state) {
    int dist = 0;
    uint8_t tile;
    pair<uint8_t, uint8_t> pos_now;
    pair<uint8_t, uint8_t> pos_goal;

    for (int i = 0; i < state.size; i++) {
        tile = state.getTileAt(i);
        if (tile != 0) {
            pos_now = state.idxPos(i);
            pos_goal = state.idxPos(tile);
            dist += abs(pos_now.first - pos_goal.first) +
                    abs(pos_now.second - pos_goal.second);
        }
    }

    this->called_count++;
    this->accumulator += dist;

    return dist;
}
