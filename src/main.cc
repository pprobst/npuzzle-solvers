#include "state.h"
#include "search.h"
#include "astar.h"
#include "bfs-graph.h"
#include "gbfs.h"
#include "idastar.h"
#include "ids.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>

using namespace std;

void printSolution(Search::Solution solution, char* algorithm, int state_size, bool record);
void printUnsolvable(char* algorithm, int state_size, bool record);

int main(int argc, char** argv) {
    string input;
    vector<uint8_t> curr_problem;
    vector<State> problems;

    if (argc <= 2) {
      cout << "Incorrect arguments. Run as:\n$ ./main -<astar/bfs/idastar/idfs/gbfs> <instance_1>, <instance_n>" << endl;
      cout << "Example:\n$ ./main -astar 0 6 1 7 4 2 3 8 5, 5 0 2 6 4 8 1 7 3" << endl;
      return 1;
    }

    else {
        char* algorithm = argv[1];

        for (int i = 2; i < argc; i++) {
            int arg_len = char_traits<char>::length(argv[i]);

            if ((arg_len == 2 && argv[i][1] == ',') || (i == argc-1)) {
                curr_problem.push_back(stoi(argv[i]));
                problems.push_back(State::makeInitialState(curr_problem));
                curr_problem.clear();
            }

            else {
                curr_problem.push_back(stoi(argv[i]));
            }
        }

        optional<Search::Solution> solution;
        bool record = false;

        if (!strcmp(algorithm, "-bfs")) {
            for (State initial_state : problems) {
                BFSGraph bfs = BFSGraph();
                solution = bfs.run(initial_state);
                if (solution) printSolution(solution.value(), algorithm, initial_state.size, record);
                else printUnsolvable(algorithm, initial_state.size, record);
            }
        }
        else if (!strcmp(algorithm, "-idfs")) {
            for (State initial_state : problems) {
                IDS ids = IDS();
                solution = ids.run(initial_state);
                if (solution) printSolution(solution.value(), algorithm, initial_state.size, record);
                else printUnsolvable(algorithm, initial_state.size, record);
            }
        }
        else if (!strcmp(algorithm, "-astar")) {
            for (State initial_state : problems) {
                AStar astar = AStar(Heuristic());
                solution = astar.run(initial_state);
                if (solution) printSolution(solution.value(), algorithm, initial_state.size, record);
                else printUnsolvable(algorithm, initial_state.size, record);
            }
        }
        else if (!strcmp(algorithm, "-idastar")) {
            for (State initial_state : problems) {
                IDAStar idastar = IDAStar(Heuristic());
                solution = idastar.run(initial_state);
                if (solution) printSolution(solution.value(), algorithm, initial_state.size, record);
                else printUnsolvable(algorithm, initial_state.size, record);
            }
        }
        else if (!strcmp(algorithm, "-gbfs")) {
            for (State initial_state : problems) {
                GBFS gbfs = GBFS(Heuristic());
                solution = gbfs.run(initial_state);
                if (solution) printSolution(solution.value(), algorithm, initial_state.size, record);
                else printUnsolvable(algorithm, initial_state.size, record);
            }
        }
    }

    return 0;
}

void printSolution(Search::Solution solution, char* algorithm, int state_size, bool record) {
    cout << solution.expanded_nodes << ","<< solution.optimal_solution_length
         << "," << fixed << solution.solution_time << "," << solution.heuristic_mean_value
         << ","<< solution.heuristic_start_state << endl;

    if (record) {
        string filename = "results/";
        filename += algorithm+1;
        filename += "-" + to_string(state_size-1) + "p" + ".csv";
        ofstream outfile;
        outfile.open(filename, ios_base::app);
        outfile << solution.expanded_nodes << ","<< solution.optimal_solution_length
                <<  "," << fixed << solution.solution_time << ","
                << solution.heuristic_mean_value
                << ","<< solution.heuristic_start_state << endl;
    }
}

void printUnsolvable(char* algorithm, int state_size, bool record) {
    cout << "-" << ","<< "-" << "," << "-" << "," << "-" << ","<< "-" << endl;

    if (record) {
        string filename = "results/";
        filename += algorithm+1;
        filename += "-" + to_string(state_size-1) + "p" + ".csv";
        ofstream outfile;
        outfile.open(filename, ios_base::app);
        outfile << "-" << ","<< "-" << "," << "-" << "," << "-" << ","<< "-" << endl;
    }
}
