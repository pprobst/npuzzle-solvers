#pragma once
#ifndef GBFS_H
#define GBFS_H

#include "search.h"

class GBFS : public Search {
    public:
    GBFS(Heuristic h);
    std::optional<Solution> run(State state);
};

#endif
