#pragma once
#ifndef ASTAR_H
#define ASTAR_H

#include "search.h"

class AStar : public Search {
  public:
    AStar(Heuristic h);
    std::optional<Solution> run(State state);
};

#endif
