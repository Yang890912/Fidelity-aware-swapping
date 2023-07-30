#ifndef __GRAPH_H
#define __GRAPH_H

#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <random>
#include <functional>
#include <cassert>
#include <ctime>
#include <algorithm>
#include "../Node/Node.h"
#include "../Shape/Shape.h"
#include "../../config.h"

using namespace std;

class Graph {
    int num_nodes;
    int time_limit;
    double A, B, n, T, tao;
    double fidelity_gain;
    int usage;
    int succ_request_cnt;
    vector<Node> nodes;
    vector<vector<int>> adj_list;
    vector<set<int>> adj_set;
    vector<double> boundary, cnt;
public:
    Graph(int _num_nodes, int _time_limit, int memory_lower_bound, int memory_upper_bound, double _A, double _B, double _n, double _T, double _tao);
    ~Graph();
    int get_node_memory_at(int node_id, int t);
    int get_node_memory(int node_id);
    int get_num_nodes();
    int get_time_limit();
    int get_succ_request_cnt();
    int get_memory_total();
    int get_usage();

    double get_A();
    double get_B();
    double get_n();
    double get_T();
    double get_tao();
    double get_fidelity_gain();

    vector<double> get_boundary();
    vector<double> get_cnt();
    bool check_resource(Shape shape);
    void reserve_shape(Shape shape);
    vector<int> get_path(int from, int to);
};

#endif