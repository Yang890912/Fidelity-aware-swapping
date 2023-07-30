#include <iostream>
#include <queue>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <omp.h>
#include <ctime>
#include "Network/Graph/Graph.h"
#include "Algorithm/AlgorithmBase/AlgorithmBase.h"
#include "Algorithm/MyAlgo1/MyAlgo1.h"
#include "Algorithm/MyAlgo2/MyAlgo2.h"
#include "Algorithm/MyAlgo3/MyAlgo3.h"
#include "Algorithm/MyAlgo4/MyAlgo4.h"
#include "Algorithm/MyAlgo5/MyAlgo5.h"

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
    string file_path = "../data/";

    map<string, double> default_setting;
    default_setting["num_nodes"] = 100;
    default_setting["request_cnt"] = 30;
    default_setting["time_limit"] = 8;
    default_setting["avg_memory"] = 7;
    default_setting["tao"] = 0.2;

    map<string, vector<double>> change_parameter;
    change_parameter["time_limit"] = {6, 22};
    vector<double> boundary = Graph(100, 8, 7, 7, 0.25, 0.75, 2, 10, 0.2).get_boundary();
    int round = 20;

    vector<map<string, map<string, vector<double>>>> result(round);
    vector<string> X_names = {"time_limit"};
    vector<string> algo_names = {"MyAlgo1", "MyAlgo3", "Merge", "Linear"};


    for(string X_name : X_names) {
        for(double change_value : change_parameter[X_name]) {
            map<string, double> input_parameter = default_setting;
            input_parameter[X_name] = change_value;
            int num_nodes = input_parameter["num_nodes"];
            int avg_memory = input_parameter["avg_memory"];
            int memory_up = avg_memory + 2;
            int memory_lb = avg_memory - 2;
            int request_cnt = input_parameter["request_cnt"];
            int time_limit = input_parameter["time_limit"];
            #pragma omp parallel for
            for(int r = 0; r < round; r++){
                string round_str = to_string(r);
                // string filename = file_path + "input/round_" + round_str + ".input";
                // string command = "python3 main.py ";
                // string parameter = to_string(num_of_node) + " " + to_string(min_channel_cnt) + " " + to_string(max_channel_cnt) + " " + to_string(min_memory_cnt) + " " + to_string(max_memory_cnt) + " " + to_string(min_fidelity) + " " + to_string(max_fidelity) + " " + to_string(social_density) + " " + to_string(area_alpha);
                // if(system((command + filename + " " + parameter).c_str()) != 0){
                //     cerr<<"error:\tsystem proccess python error"<<endl;
                //     exit(1);
                // }
                double A = 0.25, B = 0.75, tao = input_parameter["tao"], T = 10, n = 2;

                Graph graph(num_nodes, time_limit, memory_lb, memory_up, A, B, n, T, tao);
                boundary = graph.get_boundary();
                vector<pair<int, int>> requests;
                for(int i = 0; i < request_cnt; i++) {
                    pair<int, int> new_request = generate_new_request(num_nodes);
                    int len = (int)graph.get_path(new_request.first, new_request.second).size();
                    while(len <= 4 || len >= 40) {
                        new_request = generate_new_request(num_nodes);
                        len = (int)graph.get_path(new_request.first, new_request.second).size();
                    }
                    requests.push_back(new_request);
                }

                vector<AlgorithmBase*> algorithms;
                algorithms.emplace_back(new MyAlgo1(graph, requests));
                algorithms.emplace_back(new MyAlgo3(graph, requests));
                algorithms.emplace_back(new MyAlgo4(graph, requests));
                algorithms.emplace_back(new MyAlgo5(graph, requests));


                #pragma omp parallel for
                for(int i = 0; i < (int)algorithms.size(); i++) {
                    algorithms[i]->run();
                    result[r][algorithms[i]->get_name()]["cdf"] = algorithms[i]->get_cdf();
                }
            
                for(auto &algo : algorithms){
                    delete algo;
                }
                algorithms.clear();
            
            }
            
            string filename = "ans/" + X_name + to_string(change_value) + "_" + "cdf" + ".ans";
            ofstream ofs;
            ofs.open(file_path + filename, ios::out);
            map<string, vector<double>> sum_cdf;
            for(string algo_name : algo_names){
                sum_cdf[algo_name].clear();
                sum_cdf[algo_name].resize(boundary.size(), 0);
                for(int r = 0; r < round; r++){
                    for(int i = 0; i < (int)boundary.size(); i++) {
                        sum_cdf[algo_name][i] += result[r][algo_name]["cdf"][i];
                    }
                }
            }
            for(int i = 0; i < (int)boundary.size(); i++) {
                ofs << boundary[i] << ' ';
                for(string algo_name : algo_names) {
                    ofs << sum_cdf[algo_name][i] / (double)(request_cnt * round) << ' ';
                }
                ofs << endl;
            }
            ofs << endl;
            ofs.close();
        }
    }

    return 0;
}