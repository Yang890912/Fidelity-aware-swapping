#ifndef __ALGORITHMBASE_H
#define __ALGORITHMBASE_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "../../Network/Graph/Graph.h"
#include "../../config.h"
using namespace std;


class AlgorithmBase {
protected:
    string algorithm_name;
    Graph graph;
    map<string, double> res;
    vector<pair<int, int>> requests;
    int time_limit;
    double A, B, n, T, tao;
    double bar(double F);
    double Fswap(double Fa, double Fb);
    double t2F(double t);
    double F2t(double F);
    double pass_tao(double F);
public:
    AlgorithmBase(Graph graph, vector<pair<int, int>> requests);
    map<string, double> get_res();
    double get_res(string str);
    string get_name();
    virtual ~AlgorithmBase();
    virtual void run() = 0;
};

#endif