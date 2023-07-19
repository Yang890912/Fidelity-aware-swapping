#include "AlgorithmBase.h"

AlgorithmBase::AlgorithmBase(Graph graph, vector<pair<int, int>> requests):
    graph(graph), requests(requests) {
    time_limit = graph.get_time_limit();
    A = graph.get_A();
    B = graph.get_B();
    n = graph.get_n();
    T = graph.get_T();
    tao = graph.get_tao();
}

AlgorithmBase::~AlgorithmBase() {
    if(DEBUG) cerr << "delete AlgorithmBase" << endl;
}

map<string, double> AlgorithmBase::get_res() {
    return res;
}

double AlgorithmBase::bar(double F) {
    return (1.0 - F);
}
double AlgorithmBase::Fswap(double Fa, double Fb) {
    return Fa * Fb + (1.0 / 3.0) * bar(Fa) * bar(Fb);
}
double AlgorithmBase::t2F(double t) {
    if(t >= 1e5) return 0; 
    return A + B * exp(-pow(t / T, n));
}
double AlgorithmBase::F2t(double F) {
    if(F <= EPS) return 1e9;
    return T * pow(-log((F - A) / B), 1.0 / n);
}

double AlgorithmBase::pass_tao(double F) {
    return t2F(F2t(F) + tao);
}