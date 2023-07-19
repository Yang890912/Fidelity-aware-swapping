#include "Node.h"

Node::Node(int id, int memory, int time_limit): id(id), memory(memory), time_limit(time_limit) {
    memory_table.resize(time_limit, memory);
    // cerr << "memory = " << memory << endl;
}

Node::~Node() {
    if(DEBUG) cerr << "Delete Node " << id << endl;
}
int Node::get_id() {
    return id;
}

int Node::get_memory() {
    return memory;
}

int Node::get_memory_at(int t) {
    return memory_table[t];
}

vector<int> Node::get_neighbot() {
    return neighbor;
}

void Node::add_neighbor(int neighbor_id) {
    neighbor.push_back(neighbor_id);
}

void Node::reserve_memory(int t, int amount) {
    if(memory_table[t] < amount) {
        cerr << "error: memory is not enough" << endl;
        exit(1);
    }
    memory_table[t] -= amount;
}