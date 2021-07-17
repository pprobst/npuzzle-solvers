#include "node.h"

using namespace std;

uint64_t Node::node_counter = 0;

Node::Node() {
    this->parent_state = nullptr;
    this->state = State();
    this->action = Action::NONE;
    this->idx = 0;
    this->cost = 0;
    this->heuristic = 0;
}

Node::Node(State* parent_s, Action a, const State& s, uint32_t pcost, Heuristic& h) {
    this->parent_state = parent_s;
    this->state = s;
    this->action = a;
    this->cost = pcost+1;
    this->idx = Node::node_counter++;
    this->heuristic = h.calculate(s);
}

Node::Node(State* parent_s, Action a, const State& s, uint32_t pcost) {
    this->parent_state = parent_s;
    this->state = s;
    this->action = a;
    this->cost = pcost+1;
    this->idx = Node::node_counter++;
    this->heuristic = 0;
}

void Node::makeRootNode(const State& s, Heuristic& h) {
    this->state = s;
    this->cost = 0;
    this->idx = Node::node_counter++;
    this->heuristic = h.calculate(s);
}

void Node::makeRootNode(const State& s) {
    this->state = s;
    this->cost = 0;
    this->idx = Node::node_counter++;
    this->heuristic = 0;
}
