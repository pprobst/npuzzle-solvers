# npuzzle-solvers
Implementation of various search algorithms to solve 8-Puzzle and 15-Puzzle instances. Written for theoretical purposes, but they're still reasonably efficient: A\*, for example, can solve 84/100 of the provided 15-Puzzle instances with a time limit of 30s and a RAM limit of 8 GB for each instance.

The heuristic used is the Manhattan distance, _without_ linear conflicts. 

The problem instances are located in the `npuzzle` directory. 

## Implemented algorithms

* Breadth-first search graph
* Greedy best-first search
* A\*
* IDA\*
* Iterative deepening depth-first search

## Execution

`make`

`./main -<astar/bfs/idastar/idfs/gbfs> <instance_1>, <instance_n>`

#### Example:

`./main -idastar 0 6 1 7 4 2 3 8 5, 5 0 2 6 4 8 1 7 3, 2 4 7 0 3 6 8 1 5`

**Output:** 

expanded nodes, solution length, search time (s), average heuristic value, initial state heuristic value

```
165,16,0.000032,7.096774,10
439,21,0.000074,10.747945,11
724,23,0.000152,11.710304,15
```

Each output line corresponds to an instance provided in the input, in order.

The results can be optionally recorded in `csv` files by changing the value of
variable `record` in `main.cc` from `false` to `true`.
