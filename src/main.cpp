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
    default_setting["time_limit"] = 14;
    default_setting["avg_memory"] = 7;
    default_setting["tao"] = 0.2;
    default_setting["path_length"] = -1;


    map<string, vector<double>> change_parameter;
    change_parameter["request_cnt"] = {4, 17, 30, 43, 56};
    change_parameter["num_nodes"] = {40, 70, 100, 130, 160};
    // change_parameter["time_limit"] = {10, 12, 14, 16, 18};
    change_parameter["time_limit"] = {6, 22};
    change_parameter["avg_memory"] = {3, 5, 7, 9, 11};
    change_parameter["tao"] = {0.2, 0.4, 0.6, 0.8, 1};
    change_parameter["path_length"] = {11, 12, 13, 14};


    // vector<string> X_names = {"time_limit", "request_cnt", "num_nodes", "avg_memory", "tao"};
    vector<string> X_names = {"path_length", "request_cnt"};
    vector<string> Y_names = {"fidelity_gain", "succ_request_cnt", "utilization"};
    vector<string> algo_names = {"MyAlgo1", "MyAlgo2", "MyAlgo3", "Merge", "Linear"};
    // init result
    for(string X_name : X_names) {
        for(string Y_name : Y_names){
            string filename = "ans/" + X_name + "_" + Y_name + ".ans";
            fstream file( file_path + filename, ios::out );
        }
    }
    

    int round = 50;
    vector<Graph> default_graphs;
    vector<vector<pair<int, int>>> default_requestss;
    for(int r = 0; r < round; r++) {

        int num_nodes = default_setting["num_nodes"];
        int avg_memory = default_setting["avg_memory"];
        int memory_up = avg_memory + 2;
        int memory_lb = avg_memory - 2;
        // int request_cnt = default_setting["request_cnt"];
        int time_limit = default_setting["time_limit"];
        double A = 0.25, B = 0.75, tao = default_setting["tao"], T = 10, n = 2;
        default_graphs.push_back(Graph(num_nodes, time_limit, memory_lb, memory_up, A, B, n, T, tao));
        vector<pair<int, int>> default_requests;
        for(int i = 0; i < 100; i++) {

            pair<int, int> new_request = generate_new_request(100);
            while((int)default_graphs[r].get_path(new_request.first, new_request.second).size() <= 4) {
                new_request = generate_new_request(100);
            }
            default_requests.push_back(new_request);
        }
        default_requestss.push_back(default_requests);
    }
    for(string X_name : X_names) {
        map<string, double> input_parameter = default_setting;

        for(double change_value : change_parameter[X_name]) {
            vector<map<string, map<string, double>>> result(round);
            input_parameter[X_name] = change_value;
            
            int num_nodes = input_parameter["num_nodes"];
            int avg_memory = input_parameter["avg_memory"];
            int memory_up = avg_memory + 1;
            int memory_lb = avg_memory - 1;
            int request_cnt = input_parameter["request_cnt"];
            int time_limit = input_parameter["time_limit"];
            int length_upper, length_lower;
            if(input_parameter["path_length"] == -1) {
                length_upper = num_nodes;
                length_lower = 5;
            } else {
                length_upper = input_parameter["path_length"] + 1;
                length_lower = input_parameter["path_length"] - 1;
            }

            #pragma omp parallel for
            for(int r = 0; r < round; r++){
                string round_str = to_string(r);
                ofstream ofs;
                ofs.open(file_path + "log/" + X_name + "_in_" + to_string(change_value) + "_Round_" + round_str + ".log");

                time_t now = time(0);
                char* dt = ctime(&now);
                cerr  << "時間 " << dt << endl << endl; 
                ofs << "時間 " << dt << endl << endl; 

                // string filename = file_path + "input/round_" + round_str + ".input";
                // string command = "python3 main.py ";
                // string parameter = to_string(num_of_node) + " " + to_string(min_channel_cnt) + " " + to_string(max_channel_cnt) + " " + to_string(min_memory_cnt) + " " + to_string(max_memory_cnt) + " " + to_string(min_fidelity) + " " + to_string(max_fidelity) + " " + to_string(social_density) + " " + to_string(area_alpha);
                // if(system((command + filename + " " + parameter).c_str()) != 0){
                //     cerr<<"error:\tsystem proccess python error"<<endl;
                //     exit(1);
                // }
                double A = 0.25, B = 0.75, tao = input_parameter["tao"], T = 10, n = 2;

                Graph graph(num_nodes, time_limit, memory_lb, memory_up, A, B, n, T, tao);
                if(X_name == "request_cnt") {
                    graph = default_graphs[r];
                }

                ofs << "--------------- in round " << r << " -------------" <<endl;
                vector<pair<int, int>> requests;
                for(int i = 0; i < request_cnt; i++) {
                    if(X_name == "request_cnt") {
                        requests.push_back(default_requestss[r][i]);
                        continue;
                    }
                    pair<int, int> new_request = generate_new_request(num_nodes);
                    int len = graph.get_path(new_request.first, new_request.second).size();
                    int cnt = 1000;
                    while(len < length_lower || len > length_upper) {
                        new_request = generate_new_request(num_nodes);
                        len = graph.get_path(new_request.first, new_request.second).size();
                        if(cnt == 0) break;
                        cnt--;
                    }
                    while((int)graph.get_path(new_request.first, new_request.second).size() <= 3) {
                        new_request = generate_new_request(num_nodes);
                    }
                    requests.push_back(new_request);
                }

                vector<AlgorithmBase*> algorithms;
                algorithms.emplace_back(new MyAlgo1(graph, requests));
                algorithms.emplace_back(new MyAlgo2(graph, requests));
                algorithms.emplace_back(new MyAlgo3(graph, requests));
                algorithms.emplace_back(new MyAlgo4(graph, requests));
                algorithms.emplace_back(new MyAlgo5(graph, requests));


                #pragma omp parallel for
                for(int i = 0; i < (int)algorithms.size(); i++) {
                    algorithms[i]->run();
                }


                for(int i = 0; i < (int)algorithms.size(); i++) {
                    for(string Y_name : Y_names) {
                        result[r][algorithms[i]->get_name()][Y_name] = algorithms[i]->get_res(Y_name);
                    }
                }                

                now = time(0);
                dt = ctime(&now);
                cerr << "時間 " << dt << endl << endl;
                ofs << "時間 " << dt << endl << endl; 
                ofs.close();
            
                for(auto &algo : algorithms){
                    delete algo;
                }
                algorithms.clear();
            
            }
            
            map<string, map<string, double>> sum_res;
            // for(string algo_name : algo_names){
            //     for(int r = 0; r < round; r++){
            //         result[r][algo_name]["waiting_time"] /= result[T][algo_name]["total_request"];
            //         result[r][algo_name]["encode_ratio"] = result[T][algo_name]["encode_cnt"] / (result[T][algo_name]["encode_cnt"] + result[T][algo_name]["unencode_cnt"]);
            //         result[r][algo_name]["succ-finished_ratio"] = result[T][algo_name]["throughputs"] / result[T][algo_name]["finished_throughputs"];
            //         result[r][algo_name]["fail-finished_ratio"] = 1 - result[T][algo_name]["succ-finished_ratio"];
            //         result[r][algo_name]["path_length"] = result[T][algo_name]["path_length"] / result[T][algo_name]["finished_throughputs"];
            //         result[r][algo_name]["divide_cnt"] = result[T][algo_name]["divide_cnt"] / result[T][algo_name]["finished_throughputs"];
            //         result[r][algo_name]["use_memory_ratio"] = result[T][algo_name]["use_memory"] / result[T][algo_name]["total_memory"];
            //         result[r][algo_name]["use_channel_ratio"] = result[T][algo_name]["use_channel"] / result[T][algo_name]["total_channel"];
            //     }
            // }

            for(string Y_name : Y_names) {
                string filename = "ans/" + X_name + "_" + Y_name + ".ans";
                ofstream ofs;
                ofs.open(file_path + filename, ios::app);
                ofs << change_value << ' ';
                
                for(string algo_name : algo_names){
                    for(int r = 0; r < round; r++){
                        sum_res[algo_name][Y_name] += result[r][algo_name][Y_name];
                    }
                    ofs << sum_res[algo_name][Y_name] / round << ' ';
                }
                ofs << endl;
                ofs.close();
            }
        }
    }
    return 0;
}