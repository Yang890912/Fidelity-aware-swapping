#include "MyAlgo5.h"

MyAlgo5::MyAlgo5(Graph graph, vector<pair<int, int>> requests):
    AlgorithmBase(graph, requests) {
    algorithm_name = "MyAlgo5";
    // m = i + vt
    // x(i, m) = 0
    // delta = (1 + eps)((1 + eps)m)^(-1/eps)
    // alpha(i) = delta 
    // beta(v, t) = delta / C(v)

    epsilon = (0.1);
    double m = requests.size() + (double)graph.get_num_nodes() * (double)graph.get_time_limit();
    double delta = (1 + epsilon) * (1.0 / pow((1 + epsilon) * m, 1.0 / epsilon));
    obj = m * delta;
    cerr << "[MyAlgo5] delta = " << delta << endl;
    x.resize(requests.size());
    alpha.resize(requests.size(), delta);
    beta.resize(graph.get_num_nodes(), vector<double>(graph.get_time_limit()));

    for(int i = 0; i < graph.get_num_nodes(); i++) {
        for(int t = 0; t < graph.get_time_limit(); t++) {
            beta[i][t] = delta / graph.get_node_memory_at(i, t);
        }
    }
}


Shape_vector MyAlgo5::separation_oracle() {
    Shape_vector min_shape;
    double min_value = INF;
    for(int i = 0; i < (int)requests.size(); i++) {
        int src = requests[i].first, dst = requests[i].second;
        // cerr << "[MyAlgo1] " << "path len = " << graph.get_path(src, dst).size() << endl;
        auto result = find_min_shape(src, dst);
        Shape_vector shape = result.first;
        double value = result.second + alpha[i];
        if(value < min_value) {
            min_shape = shape;
            min_value = value;
        }
        // cerr << "[MyAlgo1] " << "find shape" << endl;
    }
    return min_shape;
}
pair<Shape_vector, double> MyAlgo5::find_min_shape(int src, int dst) {
    vector<int> path = graph.get_path(src, dst);
    
    dp.clear();
    dp.resize(path.size());
    par.clear();
    par.resize(path.size());
    caled.clear();
    caled.resize(path.size());
    for(int i = 0; i < (int)path.size(); i++) {
        dp[i].resize(path.size());
        par[i].resize(path.size());
        caled[i].resize(path.size());
        for(int j = 0; j < (int)path.size(); j++) {
            dp[i][j].resize(time_limit);
            par[i][j].resize(time_limit, -2);
            caled[i][j].resize(time_limit, false);
        }
    }

    double best = INF;
    int best_time = -1;
    for(int t = 0; t < time_limit; t++) {
        double result = recursion_calculate_min_shape(0, path.size() - 1, t, path);
        if(best > result) {
            best = result;
            best_time = t;
        }
    }

    if(best_time == -1) return {{}, INF};

    return {recursion_find_shape(0, (int)path.size() - 1, best_time, path), best};
}
double MyAlgo5::recursion_calculate_min_shape(int left, int right, int t, vector<int> &path) {
    if(t <= 0) return INF;
    // cerr << left << " " << right << " " << t << " " << (int)path.size() << endl;

    int left_id = path[left], right_id = path[right];
    if(left == right - 1) {
        return beta[left_id][t - 1] + beta[left_id][t] + beta[right_id][t - 1] + beta[right_id][t];
    }

    if(caled[left][right][t]) return dp[left][right][t];
    
    double best = recursion_calculate_min_shape(left, right, t - 1, path);
    int best_k = -1;
    for(int k = left + 1; k < right; k++) {
        double left_result = recursion_calculate_min_shape(left, k, t - 1, path);
        double right_result = recursion_calculate_min_shape(k, right, t - 1, path);
        double result = left_result + right_result;
        if(result < best) {
            best = result;
            best_k = k;
        }
    }

    caled[left][right][t] = true;
    par[left][right][t] = best_k;
    return dp[left][right][t] = best + beta[left_id][t] + beta[right_id][t];
}
Shape_vector MyAlgo5::recursion_find_shape(int left, int right, int t, vector<int> &path) {
    int left_id = path[left], right_id = path[right], k = par[left][right][t];
    if(left == right - 1 && k == -2) {
        Shape_vector result;
        result.push_back({left_id, {{t - 1, t}}});
        result.push_back({right_id, {{t - 1, t}}});
        return result;    
    }

    if(k == -1) {
        Shape_vector last_time = recursion_find_shape(left, right, t - 1, path);
        if(DEBUG) {
            assert(last_time.front().first == left_id);
            assert(last_time.front().second[0].second == t - 1);
            assert(last_time.back().first == right_id);
            assert(last_time.back().second[0].second == t - 1);
        }
        last_time.front().second[0].second++;
        last_time.back().second[0].second++;
        return last_time;
    }

    assert(k >= 0);
    Shape_vector left_result, right_result, result;

    int k_id = path[k];
    left_result = recursion_find_shape(left, k, t - 1, path);
    right_result = recursion_find_shape(k, right, t - 1, path);

    if(DEBUG) {
        assert(left_result.front().first == left_id);
        assert(left_result.front().second[0].second == t - 1);
        assert(left_result.front().second.size() == 1);
        assert(left_result.back().first == k_id);
        assert(right_result.front().first == k_id);
        assert(right_result.back().first == right_id);
        assert(right_result.back().second[0].second == t - 1);
        assert(left_result.back().second.size() == 1);
    }

    for(int i = 0; i < (int)left_result.size(); i++) {
        result.push_back(left_result[i]);
    }

    result.back().second.push_back(right_result.front().second.front());

    for(int i = 1; i < (int)right_result.size(); i++) {
        result.push_back(right_result[i]);
    }

    result.front().second[0].second++;
    result.back().second[0].second++;
    return result;
}

void MyAlgo5::run() {

    // q = min (1, C(v) / theta(v, t)) for all v, t

    // primal:
    // x(i, m) = x(i, m) + q

    // dual:
    // alpha(v) = alpha(v)(1 + eps(q / ahpla(v))
    // beta(v, t) = beta(v, t)(1 + eps(q / beta(v, t))

    while(obj < 1.0) {
        Shape_vector shape = separation_oracle();
        if(shape.empty()) break;
        double q = 1;
        for(int i = 0; i < (int)shape.size(); i++) {
            map<int, int> need_amount; // time to amount
            for(pair<int, int> rng : shape[i].second) {
                int left = rng.first, right = rng.second;
                for(int t = left; t <= right; t++) {
                    need_amount[t]++;
                }
            }

            for(pair<int, int> P : need_amount) {
                int t = P.first;
                double theta = P.second;
                q = min(q, graph.get_node_memory_at(i, t) / theta);
            }
        }


        int request_index = -1;
        for(int i = 0; i < (int)requests.size(); i++) {
            if(requests[i] == make_pair(shape.front().first, shape.back().first)) {
                request_index = i;
            }
        }

        x[request_index][shape] += q;
    
        double ori = alpha[request_index];
        alpha[request_index] = alpha[request_index] * (1 + epsilon * q);
        obj += (alpha[request_index] - ori);

        for(int i = 0; i < (int)shape.size(); i++) {
            map<int, int> need_amount; // time to amount
            for(pair<int, int> rng : shape[i].second) {
                int left = rng.first, right = rng.second;
                for(int t = left; t <= right; t++) {
                    need_amount[t]++;
                }
            }

            for(pair<int, int> P : need_amount) {
                int t = P.first;
                int node_id = shape[i].first;
                double theta = P.second;
                double original = beta[node_id][t];
                beta[node_id][t] = beta[node_id][t] * (1 + epsilon * (q / (graph.get_node_memory_at(node_id, t) / theta)));
                obj += (beta[node_id][t] - original) * theta;
            }
        }

        // cerr << "[MyAlgo1] obj = " << obj << endl;
    }

    double max_xim_sum = 0;
    for(int i = 0; i < (int)requests.size(); i++) {
        double xim_sum = 0;
        for(auto P : x[i]) {
            xim_sum += P.second;
            res["fidelity_gain"] += Shape(P.first).get_fidelity(A, B, n, T, tao);
            res["succ_request_cnt"] += P.second;
        }
        max_xim_sum = max(max_xim_sum, xim_sum);
    }

    res["succ_request_cnt"] /= max_xim_sum;
    res["fidelity_gain"] /= max_xim_sum;

    cerr << "[" << algorithm_name << "] end" << endl;
}