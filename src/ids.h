#pragma once
#ifndef IDS_H
#define IDS_H

#include "search.h"

// Iterative Deepening Search
class IDS : public Search {
    public:
    IDS();
	//optional<std::deque<Action>> DLS(Node& n, int depth_limit);
	bool DLS(Node& n, int depth_limit);
    std::optional<Solution> run(State state);
};

#endif
