#pragma once
#ifndef BFSGRAPH_H
#define BFSGRAPH_H

#include "search.h"

class BFSGraph : public Search {
  public:
    BFSGraph();
    std::optional<Solution> run(State state);
};

#endif
