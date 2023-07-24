#include "MyAlgo3.h"

MyAlgo3::MyAlgo3(Graph graph, vector<pair<int, int>> requests):
    AlgorithmBase(graph, requests) {
    algorithm_name = "MyAlgo3";
}



pair<Shape, double> MyAlgo3::calculate_best_shape(int src, int dst) {
    // cerr << "cal " << src << " " << dst << endl;
    vector<int> path = graph.get_path(src, dst);
    dp.clear();
    dp.resize(path.size());
    caled.clear();
    caled.resize(path.size());
    par.clear();
    par.resize(path.size());
    for(int i = 0; i < (int)path.size(); i++) {
        dp[i].resize(path.size());
        par[i].resize(path.size());
        caled[i].resize(path.size());
        for(int j = 0; j < (int)path.size(); j++) {
            dp[i][j].resize(time_limit);
            par[i][j].resize(time_limit);
            caled[i][j].resize(time_limit);
            for(int t = 0; t < time_limit; t++) {
                dp[i][j][t].resize(4, 0);
                par[i][j][t].resize(4, {-2, -2});
                caled[i][j][t].resize(4, false);
            }
        }
    }

    double best = EPS;
    int best_time = -1;
    for(int t = time_limit - 1; t >= 0; t--) {
        double result = solve_fidelity(0, path.size() - 1, t, 0, path);
        if(result > best) {
            best_time = t;
            best = result;
        }
    }

    if(best_time == -1) return {{}, 0};
    Shape shape = Shape(backtracing_shape(0, path.size() - 1, best_time, 0, path));
    // shape.print();
    if(fabs(shape.get_fidelity(A, B, n, T, tao) - best) > EPS) {
        shape.print();
        cerr << "[" << algorithm_name << "]" << endl;
        cerr << shape.get_fidelity(A, B, n, T, tao) << " " << best << endl;
        cerr << "the result diff is too much" << endl;
        exit(1);
    }
    return {shape, best};
}

// state = 0, left right no limit
// state = 1, left limit
// state = 2, right limit
// state = 3, left and right limit
double MyAlgo3::solve_fidelity(int left, int right, int t, int state, vector<int> &path) {
    int left_id = path[left], right_id = path[right];
    int left_remain = graph.get_node_memory_at(left_id, t);
    int right_remain = graph.get_node_memory_at(right_id, t);
    if(state == 0 && (left_remain <= 0 || right_remain <= 0)) return 0;
    if(state == 1 && (left_remain <= 1 || right_remain <= 0)) return 0;
    if(state == 2 && (left_remain <= 0 || right_remain <= 1)) return 0;
    if(state == 3 && (left_remain <= 1 || right_remain <= 1)) return 0;
    
    if(t <= 0) return 0;
    if(left == right - 1) {
        int left_last_remain = graph.get_node_memory_at(left_id, t - 1);
        int right_last_remain = graph.get_node_memory_at(right_id, t - 1);
        if(state == 0 && (left_last_remain <= 0 || right_last_remain <= 0)) return 0;
        if(state == 1 && (left_last_remain <= 1 || right_last_remain <= 0)) return 0;
        if(state == 2 && (left_last_remain <= 0 || right_last_remain <= 1)) return 0;
        if(state == 3 && (left_last_remain <= 1 || right_last_remain <= 1)) return 0;
        return pass_tao(1);
    }

    if(caled[left][right][t][state]) return dp[left][right][t][state];

    double best = pass_tao(solve_fidelity(left, right, t - 1, state, path));
    pair<int, int> record = {-1, -1};

    for(int k = left + 1; k < right; k++) {
        for(int s = 1; s <= 2; s++) {
            int l_state = (state & 1) | ((s & 1) << 1);
            int r_state = (state & 2) | ((s & 2) >> 1);

            double left_result = solve_fidelity(left, k, t - 1, l_state, path);
            double right_result = solve_fidelity(k, right, t - 1, r_state, path);
            double result = Fswap(pass_tao(left_result), pass_tao(right_result));
            if(result > best) {
                best = result;
                record = {k, s};
            }
        }
    }

    caled[left][right][t][state] = true;
    par[left][right][t][state] = record;
    return dp[left][right][t][state] = best;
}

Shape_vector MyAlgo3::backtracing_shape(int left, int right, int t, int state, vector<int> &path) {
    int k = par[left][right][t][state].first;
    int s = par[left][right][t][state].second;
    int left_id = path[left], right_id = path[right];
    if(left == right - 1 && k == -2 && s == -2) {
        Shape_vector result;
        result.push_back({left_id, {{t - 1, t}}});
        result.push_back({right_id, {{t - 1, t}}});
        return result;
    }


    if(k == -1 && s == -1) {
        Shape_vector last_time = backtracing_shape(left, right, t - 1, state, path);
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

    assert(k >= 0 && s >= 0);
    Shape_vector left_result, right_result, result;

    int k_id = path[k];
    int left_state = (state & 1) | ((s & 1) << 1);
    int right_state = (state & 2) | ((s & 2) >> 1);
    left_result = backtracing_shape(left, k, t - 1, left_state, path);
    right_result = backtracing_shape(k, right, t - 1, right_state, path);

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

void MyAlgo3::run() {
    vector<pair<int, pair<int, int>>> len_requests(requests.size());
    for(int i = 0; i < (int)requests.size(); i++) {
        int length = graph.get_path(requests[i].first, requests[i].second).size();
        len_requests[i] = {length, requests[i]};
    }

    sort(len_requests.begin(), len_requests.end());

    for(int i = 0; i < (int)len_requests.size(); i++) {
        int src = len_requests[i].second.first;
        int dst = len_requests[i].second.second;
        Shape shape = calculate_best_shape(src, dst).first;
        if(!shape.get_node_mem_range().empty() && graph.check_resource(shape)) {
            graph.reserve_shape(shape);
            res["fidelity_gain"] = graph.get_fidelity_gain();
            res["succ_request_cnt"] = graph.get_succ_request_cnt();
        }
    }

    cerr << "[" << algorithm_name << "] end" << endl;
}