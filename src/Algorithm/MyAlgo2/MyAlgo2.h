#ifndef __MYALGO2_H
#define __MYALGO2_H

#include    <iostream>
#include    <algorithm>
#include    <queue>
#include    "../AlgorithmBase/AlgorithmBase.h"
#include    "../../Network/Graph/Graph.h"
#include    "../../config.h"

using namespace std;

class MyAlgo2 : public AlgorithmBase {
    vector<vector<vector<vector<double>>>> dp;
    vector<vector<vector<vector<pair<int, int>>>>> par;
    vector<vector<vector<vector<bool>>>> caled;
    pair<Shape, double> calculate_best_shape(int src, int dst);
    double solve_fidelity(int left, int right, int t, int state, vector<int> &path);
    Shape_vector backtracing_shape(int left, int right, int t, int state, vector<int> &path);
public:
    MyAlgo2(Graph graph, vector<pair<int, int>> requests);
    void run();
};

#endif