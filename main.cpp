#include <iostream>
#include <queue>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <omp.h>
#include "Network/Graph/Graph.h"
#include "Algorithm/AlgorithmBase/AlgorithmBase.h"
#include "Algorithm/MyAlgo1/MyAlgo1.h"
#include "Algorithm/MyAlgo2/MyAlgo2.h"
#include "Algorithm/MyAlgo3/MyAlgo3.h"
#include "Algorithm/MyAlgo4/MyAlgo4.h"

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
    int request_cnt = 20, num_nodes = 100, time_limit = 7;
    double A = 0.25, B = 0.75, tao = 0.2, T = 10, n = 2;
    Graph graph(num_nodes, time_limit, 4, 8, A, B, n, T, tao);

    vector<pair<int, int>> requests;
    for(int i = 0; i < request_cnt; i++) {
        pair<int, int> new_request = generate_new_request(num_nodes);
        while(graph.get_path(new_request.first, new_request.second).size() <= 10) {
            new_request = generate_new_request(num_nodes);
        }
        requests.push_back(new_request);
    }


    vector<AlgorithmBase*> algorithms;
    algorithms.emplace_back(new MyAlgo1(graph, requests));
    algorithms.emplace_back(new MyAlgo2(graph, requests));
    algorithms.emplace_back(new MyAlgo3(graph, requests));
    algorithms.emplace_back(new MyAlgo4(graph, requests));

    for(auto algo : algorithms) {
        cout << "[" << algo->get_name() << "] start" << endl; 
        algo->run();
        for(auto P : algo->get_res()) {
            cout << "[" << algo->get_name() << "] " << P.first << " " << P.second << endl;
        }
        cout << "[" << algo->get_name() << "] end" << endl; 
    }
}