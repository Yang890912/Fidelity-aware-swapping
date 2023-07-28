#ifndef __MYALGO1_H
#define __MYALGO1_H

#include <iostream>
#include <algorithm>
#include <utility>
#include "../AlgorithmBase/AlgorithmBase.h"
#include "../../Network/Graph/Graph.h"
#include "../../config.h"

using namespace std;

class MyAlgo1_time : public AlgorithmBase {
    vector<double> alpha;
    vector<vector<double>> beta;
    vector<map<Shape_vector, double>> x;
    vector<vector<vector<double>>> dp;
    vector<vector<vector<bool>>> caled;
    vector<vector<vector<int>>> par;
    double epsilon, obj;
    void variable_initialize();
    Shape_vector separation_oracle();
    pair<Shape_vector, double> find_min_shape(int src, int dst);
    double recursion_calculate_min_shape(int left, int right, int t, vector<int> &path);
    Shape_vector recursion_find_shape(int left, int right, int t, vector<int> &path);
public:
    MyAlgo1_time(Graph graph, vector<pair<int, int>> requests, double _epsilon);
    void run();
};

#endif