#pragma once
#ifndef IDASTAR_H
#define IDASTAR_H

#include "search.h"

class IDAStar : public Search {
  public:
    IDAStar(Heuristic h);
    std::pair<std::optional<int>, std::optional<int>>
    RecursiveSearch(Node &node, int f_limit);
    std::optional<Solution> run(State state);
};

#endif
