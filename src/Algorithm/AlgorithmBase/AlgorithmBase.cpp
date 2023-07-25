#include "AlgorithmBase.h"

AlgorithmBase::AlgorithmBase(Graph graph, vector<pair<int, int>> requests):
    graph(graph), requests(requests) {
    time_limit = graph.get_time_limit();
    A = graph.get_A();
    B = graph.get_B();
    n = graph.get_n();
    T = graph.get_T();
    tao = graph.get_tao();

    vector<bool> passed_node(graph.get_num_nodes(), false);
    memory_total = 0;
    for(int i = 0; i < (int)requests.size(); i++) {
        int src = requests[i].first, dst = requests[i].second;
        vector<int> path = graph.get_path(src, dst);
        for(auto node : path) {
            if(!passed_node[node]) {
                passed_node[node] = true;
                memory_total += graph.get_node_memory(node);
            }
        }
    }

    memory_total *= graph.get_time_limit();
}
AlgorithmBase::~AlgorithmBase() {
    if(DEBUG) cerr << "delete AlgorithmBase" << endl;
}

string AlgorithmBase::get_name() {
    return algorithm_name;
}

map<string, double> AlgorithmBase::get_res() {
    return res;
}

double AlgorithmBase::get_res(string str) {
    return res[str];
}

double AlgorithmBase::bar(double F) {
    return (1.0 - F);
}
double AlgorithmBase::Fswap(double Fa, double Fb) {
    if(Fa <= A + EPS || Fb <= A + EPS) return 0;
    return Fa * Fb + (1.0 / 3.0) * bar(Fa) * bar(Fb);
}
double AlgorithmBase::t2F(double t) {
    if(t >= INF / 1e2) return 0;
    return A + B * exp(-pow(t / T, n));
}
double AlgorithmBase::F2t(double F) {
    if(F <= A + EPS) return INF;
    return T * pow(-log((F - A) / B), 1.0 / n);
}
double AlgorithmBase::pass_tao(double F) {
    return t2F(F2t(F) + tao);
}