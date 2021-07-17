#pragma once
#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "state.h"

class Heuristic {
  public:
    uint32_t called_count;
    uint32_t accumulator;

    Heuristic();

    // Manhattan distance
    int calculate(const State &state);
};

#endif
