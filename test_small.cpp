#include <iostream>
#include <queue>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <omp.h>
#include "Network/Graph/Graph.h"
#include "Algorithm/AlgorithmBase/AlgorithmBase.h"
#include "Algorithm/MyAlgo2/MyAlgo2.h"

using namespace std;

pair<int, int> generate_new_request(int num_of_node){
    //亂數引擎 
    random_device rd;
    default_random_engine generator = default_random_engine(rd());
    uniform_int_distribution<int> unif(0, num_of_node-1);
    int node1 = unif(generator), node2 = unif(generator);
    while(node1 == node2) node2 = unif(generator);
    
    return make_pair(node1, node2);
}


int main(){
    int request_cnt = 10, num_of_node = 30, time_limit = 30;
    double A = 0.25, B = 0.75, tao = 0.1, T = 10, n = 2;
    Graph graph(num_of_node, time_limit, 4, 8, A, B, n, T, tao);

    vector<pair<int, int>> requests;
    for(int i = 0; i < request_cnt; i++) {
        requests.push_back(generate_new_request(num_nodes));
    }
    MyAlgo2 myalgo2(graph, requests);
    myalgo2.run();

    for(auto P : myalgo2.get_res()) {
        cout << P.first << " " << P.second << endl;
    }    
}