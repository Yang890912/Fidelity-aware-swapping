#include "Shape.h"

Shape::Shape(Shape_vector _node_mem_range):
    node_mem_range(_node_mem_range) {}

Shape::Shape() {}

Shape_vector Shape::get_node_mem_range() {
    return node_mem_range;
}


double Shape::get_fidelity(double _A, double _B, double _n, double _T, double _tao) {
    check_valid();
    A = _A, B = _B, n = _n, T = _T, tao = _tao;
    return recursion_get_fidelity(0, (int)node_mem_range.size() - 1);
}

double Shape::recursion_get_fidelity(int left, int right) {
    if(left == right - 1) return t2F(F2t(1) + tao);
    int latest = left + 1;
    for(int i = left + 1; i < right; i++) {
        if(node_mem_range[i].second[0].second > node_mem_range[latest].second[0].second) {
            latest = i;
        }
    }

    double Fa = recursion_get_fidelity(left, latest);
    double Fb = recursion_get_fidelity(latest, right);

    int now_swap_time = node_mem_range[latest].second[0].second;
    int next_swap_time = max(node_mem_range[left].second.back().second, node_mem_range[right].second.front().second);
    double pass_time = tao * (next_swap_time - now_swap_time);

    return Fswap(t2F(F2t(Fa) + pass_time), t2F(F2t(Fb) + pass_time));
}


double Shape::bar(double F) {
    return (1.0 - F);
}
double Shape::Fswap(double Fa, double Fb) {
    return Fa * Fb + (1.0 / 3.0) * bar(Fa) * (Fb);
}
double Shape::t2F(double t) {
    return A + B * exp(-pow(t / T, n));
}
double Shape::F2t(double F) {
    return pow(-pow(log((F - A) / B), 1 / n), T);
}

void Shape::check_valid() {
    if(DEBUG) cerr << "check in" << endl;
    if(node_mem_range[0].second.size() != 1) {
        cerr << "the usage of memory at src should be 1, but " << node_mem_range[0].second.size() << endl;
        exit(1);
    }
    if(node_mem_range.back().second.size() != 1) {
        cerr << "the usage of memory at dst should be 1, but " << node_mem_range.back().second.size() << endl;
        exit(1);
    }
    for(int i = 1; i < (int)node_mem_range.size() - 1; i++) {
        int node_id = node_mem_range[i].first;
        vector<pair<int, int>> memory_range = node_mem_range[i].second;
        if(memory_range.size() != 2) {
            cerr << "node " << node_id << endl;
            cerr << "the usage of memory at med node should be 2, but " << memory_range.size() << endl;
            exit(1);
        }
        
        // memory_range[[0]and memory_range[[1]
        if(memory_range.front().second != memory_range[1].second) {
            cerr << "two range should finish same time (after swap)" << endl;
            exit(1);
        }
        assert(memory_range[0].first <= memory_range[0].second);
        assert(memory_range[1].first <= memory_range[1].second);
    }

    recursion_check(0, node_mem_range.size() - 1);
    if(DEBUG) cerr << "check end" << endl;
}

void Shape::recursion_check(int left, int right) {
    if(left == right - 1) {
        int left_start = node_mem_range[left].second.back().first;
        int left_end = node_mem_range[left].second.back().second;
        int right_start = node_mem_range[right].second.front().first;
        int right_end = node_mem_range[right].second.front().second;
        assert(left_start == right_start);
        assert(left_end - 1 >= left_start);
        assert(right_end - 1 >= right_start);
        return;
    }

    int latest = left + 1;
    for(int i = left + 1; i < right; i++) {
        if(node_mem_range[i].second[0].second > node_mem_range[latest].second[0].second) {
            latest = i;
        }
    }

    int latest_finish_time = node_mem_range[latest].second[0].second;
    int left_finish_time = node_mem_range[left].second.back().second;
    int left_start_time = node_mem_range[left].second.back().first;
    int right_finish_time = node_mem_range[right].second.front().second;
    int right_start_time = node_mem_range[right].second.front().first;
    if(latest_finish_time >= left_finish_time || latest_finish_time >= right_finish_time) {
        cerr << "swap time error, swap after finish" << endl;
        cerr << "swap time = " << latest_finish_time << endl;
        cerr << "left fin = " << left_finish_time << endl;
        cerr << "right fin = " << right_finish_time << endl;
        exit(1);
    }

    if(latest_finish_time < left_start_time || latest_finish_time < right_start_time) {
        cerr << "swap time error, swap before entangle" << endl;
        exit(1);
    }

    recursion_check(left, latest);
    recursion_check(latest, right);
}

void Shape::print() {
    cerr << "length = " << node_mem_range.size() << endl;
    for(int i = 0; i < (int)node_mem_range.size(); i++) {
        int id = node_mem_range[i].first;
        cerr << "--- id = " << id << "---" << endl;
        for(auto P : node_mem_range[i].second) {
            cerr << "{" << P.first << ", " << P.second << "}" << endl;
        }
    }
}