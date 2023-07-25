#include "MyAlgo5.h"

MyAlgo5::MyAlgo5(Graph graph, vector<pair<int, int>> requests):
    AlgorithmBase(graph, requests) {
    algorithm_name = "Linear";
    linear_shape.resize(graph.get_num_nodes() + 1);
}

vector<vector<pair<int, int>>> MyAlgo5::recursion_build(int length) {
    if(!linear_shape[length].empty()) return linear_shape[length];
    if(length == 1) {
        return linear_shape[length] = {};
    }
    if(length == 2) {
        return linear_shape[length] = {{{0, 1}}, {{0, 1}}};
    }

    vector<vector<pair<int, int>>> left = recursion_build(length - 1);
    vector<vector<pair<int, int>>> right = recursion_build(2);

    int offest = left.back().front().second - right.front().front().second;
    for(int i = 0; i < (int)right.size(); i++) {
        for(int j = 0; j < (int)right[i].size(); j++) {
            right[i][j].first += offest;
            right[i][j].second += offest;
        }
    }

    for(int i = 0; i < (int)left.size(); i++) {
        linear_shape[length].push_back(left[i]);
    }

    linear_shape[length].back().push_back(right.front().front());

    for(int i = 1; i < (int)right.size(); i++) {
        linear_shape[length].push_back(right[i]);
    }

    linear_shape[length].front().front().second++;
    linear_shape[length].back().front().second++;

    // cerr << "len = " << length << endl;
    // for(int i = 0; i < (int)linear_shape[length].size(); i++) {
    //     cerr << "-----" << endl;
    //     for(int j = 0; j < (int)linear_shape[length][i].size(); j++) {
    //         cerr << "{" << linear_shape[length][i][j].first << ", " << linear_shape[length][i][j].second << "}" << endl;
    //     }
    // }
    return linear_shape[length];
}
Shape_vector MyAlgo5::build_linear_shape(vector<int> path) {
    vector<vector<pair<int, int>>> result = recursion_build(path.size());
    Shape_vector shape;
    for(int i = 0; i < (int)path.size(); i++) {
        shape.push_back({path[i], result[i]});
    }
    return shape;
}
void MyAlgo5::run() {
    for(int i = 0; i < (int)requests.size(); i++) {
        int src = requests[i].first;
        int dst = requests[i].second;
        Shape_vector shape = build_linear_shape(graph.get_path(src, dst));
        bool find = false;
        while(1) {
            bool isable = true;
            int offest = 0;
            for(int i = 0; i < (int)shape.size(); i++) {
                int node = shape[i].first;
                map<int, int> need_amount; // time to amount
                for(pair<int, int> rng : shape[i].second) {
                    int left = rng.first, right = rng.second;
                    for(int t = left; t <= right; t++) {
                        need_amount[t]++;
                    }
                }
                for(auto P : need_amount) {
                    int t = P.first, amount = P.second;
                    if(graph.get_node_memory_at(node, t) < amount) {
                        isable = false;
                        if(amount == 2 && graph.get_node_memory_at(node, t) == 1) {
                            offest = (t + 1) - max(shape[i].second.front().first, shape[i].second.back().first);
                        } else {
                            offest = (t + 1) - min(shape[i].second.front().first, shape[i].second.back().first);
                        }
                    }
                }
            }

            bool cant = false;
            for(int i = 0; i < (int)shape.size(); i++) {
                for(int j = 0; j < (int)shape[i].second.size(); j++) {
                    shape[i].second[j].first += offest;
                    shape[i].second[j].second += offest;

                    if(shape[i].second[j].second >= graph.get_time_limit()) {
                        cant = true;
                    }
                }
            }

            if(cant) break;
            
            if(isable) {
                find = true;
                break;
            }

        }

        if(find) {
            graph.reserve_shape(Shape(shape));
            res["fidelity_gain"] = graph.get_fidelity_gain();
            res["succ_request_cnt"] = graph.get_succ_request_cnt();
            res["utilization"] = graph.get_utilization();
        }
    }
    cerr << "[" << algorithm_name << "] end" << endl;
}