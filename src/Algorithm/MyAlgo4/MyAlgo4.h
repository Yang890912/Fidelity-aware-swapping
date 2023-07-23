#ifndef __MYALGO4_H
#define __MYALGO4_H

#include    <iostream>
#include    <algorithm>
#include    <queue>
#include    "../AlgorithmBase/AlgorithmBase.h"
#include    "../../Network/Graph/Graph.h"
#include    "../../config.h"

using namespace std;

class MyAlgo4 : public AlgorithmBase {
    vector<vector<vector<pair<int, int>>>> merge_shape;
    vector<vector<pair<int, int>>> recursion_build(int length);
    Shape_vector build_merge_shape(vector<int> path);

public:
    MyAlgo4(Graph graph, vector<pair<int, int>> requests);
    void run();
};

#endif