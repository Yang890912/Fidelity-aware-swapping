#include <iostream>
#include <queue>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <omp.h>
#include <ctime>
#include "Network/Graph/Graph.h"
#include "Algorithm/AlgorithmBase/AlgorithmBase.h"
#include "Algorithm/MyAlgo1_time/MyAlgo1_time.h"
#include "Algorithm/MyAlgo3/MyAlgo3.h"

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
    change_parameter["request_cnt"] = {10, 20, 30, 40, 50};

    // init result
    string filename = string("ans/") + string("runtime.ans");    

    int round = 50;

    vector<map<string, map<string, double>>> result(round);            
    int num_nodes = default_setting["num_nodes"];
    int avg_memory = default_setting["avg_memory"];
    int memory_up = avg_memory + 2;
    int memory_lb = avg_memory - 2;
    int request_cnt = default_setting["request_cnt"];
    int time_limit = default_setting["time_limit"];

    vector<double> epsilon = {0.1, 0.3, 0.5, 0.7};
    vector<string> algo_names;
    for(double eps : epsilon) {
        algo_names.push_back("MyAlgo1_time" + to_string(eps));
    }
    algo_names.push_back("MyAlgo3");

    vector<string> X_names = {"request_cnt"};

    for(string X_name : X_names) {
        string filename = "ans/" + X_name + "_" + "runtime" + ".ans";
        fstream file( file_path + filename, ios::out );
    }

    for(string X_name : X_names) {
        map<string, double> input_parameter = default_setting;

        for(double change_value : change_parameter[X_name]) {

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
                double A = 0.25, B = 0.75, tao = default_setting["tao"], T = 10, n = 2;

                Graph graph(num_nodes, time_limit, memory_lb, memory_up, A, B, n, T, tao);

                vector<pair<int, int>> requests;
                for(int i = 0; i < request_cnt; i++) {
                    pair<int, int> new_request = generate_new_request(num_nodes);
                    while((int)graph.get_path(new_request.first, new_request.second).size() <= 3) {
                        new_request = generate_new_request(num_nodes);
                    }
                    requests.push_back(new_request);
                }

                vector<AlgorithmBase*> algorithms;
                algorithms.emplace_back(new MyAlgo1_time(graph, requests, 0.1));
                algorithms.emplace_back(new MyAlgo1_time(graph, requests, 0.3));
                algorithms.emplace_back(new MyAlgo1_time(graph, requests, 0.5));
                algorithms.emplace_back(new MyAlgo1_time(graph, requests, 0.7));
                algorithms.emplace_back(new MyAlgo3(graph, requests));


                #pragma omp parallel for
                for(int i = 0; i < (int)algorithms.size(); i++) {
                    struct timespec requestStart, requestEnd;
                    clock_gettime(CLOCK_REALTIME, &requestStart);
                    algorithms[i]->run();
                    clock_gettime(CLOCK_REALTIME, &requestEnd);
                    double runtime = (requestEnd.tv_sec - requestStart.tv_sec) + (requestEnd.tv_nsec - requestStart.tv_nsec ) / 1e9;
                    result[r][algorithms[i]->get_name()]["runtime"] = runtime;
                }
            
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

            string filename = "ans/" + X_name + "_" + "runtime" + ".ans";
            ofstream ofs;
            ofs.open(file_path + filename, ios::app);
            ofs << change_value << ' ';
            for(string algo_name : algo_names){
                for(int r = 0; r < round; r++){
                    sum_res[algo_name]["runtime"] += result[r][algo_name]["runtime"];
                }
                ofs << sum_res[algo_name]["runtime"] / round << ' ';
            }
            ofs << endl;
            ofs.close();
        }
    }

    return 0;
}