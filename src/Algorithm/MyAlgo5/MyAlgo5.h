#ifndef __MYALGO5_H
#define __MYALGO5_H

#include <iostream>
#include <algorithm>
#include <utility>
#include "../AlgorithmBase/AlgorithmBase.h"
#include "../../Network/Graph/Graph.h"
#include "../../config.h"

using namespace std;

class MyAlgo5 : public AlgorithmBase {
    vector<vector<vector<pair<int, int>>>> linear_shape;
    vector<vector<pair<int, int>>> recursion_build(int length);
    Shape_vector build_linear_shape(vector<int> path);
public:
    MyAlgo5(Graph graph, vector<pair<int, int>> requests);
    void run();
};

#endif