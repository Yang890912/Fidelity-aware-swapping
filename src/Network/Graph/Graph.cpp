#include "Graph.h"


int rnd(int lower_bound, int upper_bound) {
    random_device rd;
    default_random_engine generator = default_random_engine(rd());
    uniform_int_distribution<int> unif(0, 1e9);
    return unif(generator) % (upper_bound - lower_bound + 1) + lower_bound;
}

Graph::Graph(int _num_nodes, int _time_limit, int memory_lower_bound, int memory_upper_bound, double _A, double _B, double _n, double _T, double _tao):
    num_nodes(_num_nodes), time_limit(_time_limit), A(_A), B(_B), n(_n), T(_T), tao(_tao), fidelity_gain(0), usage(0), succ_request_cnt(0) {
    // geneator an adj list
    auto gen_tree = [&](int nn) {
        vector<pair<int, int>> edge_list;
        vector<int> pru_seq, cnt(nn + 1, 0);
        for(int i = 0; i < nn - 2; i++) {
            int node = rnd(1, nn);
            pru_seq.push_back(node);
        }
        set<int> leaf;
        for(int i = 1; i <= nn; i++) {
            leaf.insert(i);
        }
        for(int i = 0; i < nn - 2; i++) {
            leaf.erase(pru_seq[i]);
            cnt[pru_seq[i]]++;
        }

        for(int i = 0; i < nn - 2; i++) {
            auto mi = leaf.begin();
            edge_list.emplace_back(*mi, pru_seq[i]);
            leaf.erase(mi);
            cnt[pru_seq[i]]--;
            if(cnt[pru_seq[i]] == 0) {
                leaf.insert(pru_seq[i]);
            }
        }
        if (leaf.size() >= 2LL) {
            edge_list.emplace_back(*leaf.begin(), *next(leaf.begin()));
        }

        if((int)edge_list.size() != nn - 1) {
            cerr << "error: In gen_tree, edge_size error" << endl;
            cerr << "num_nodes = " << nn << " edge_size = " << edge_list.size() << endl;
            exit(1);
        }

        vector<bool> vis(nn, false);
        vector<vector<int>> result_list(nn);
        for(auto P : edge_list) {
            int u = P.first, v = P.second;
            result_list[u - 1].push_back(v - 1);
            result_list[v - 1].push_back(u - 1);
        }
        // BFS to check connect
        queue<int> que;
        que.push(0);
        vis[0] = true;
        while(!que.empty()) {
            int frt = que.front();
            que.pop();
            for(int v : result_list[frt]) {
                if(!vis[v]) {
                    vis[v] = true;
                    que.push(v);
                }
            }
        }
        for(int i = 0; i < nn; i++) {
            if(!vis[i]) {
                cerr << "error: In gen_tree, tree not connected" << endl;
                exit(1);
            }
        }
        if(DEBUG) cerr << "gen tree end" << endl;
        return result_list;

    };


    adj_list = gen_tree(num_nodes);
    adj_set.clear();
    passed_node.clear();
    passed_node.resize(num_nodes, false);
    adj_set.resize(num_nodes);

    for(int i = 0; i < num_nodes; i++) {
        for(auto v : adj_list[i]) {
            adj_set[i].insert(v);
        }
    }
    assert((int)adj_list.size() == num_nodes);

    memory_total = 0;
    for(int id = 0; id < num_nodes; id++) {
        int memory_rand = rnd(memory_lower_bound, memory_upper_bound);
        nodes.push_back(Node(id, memory_rand, time_limit));
        for(int v : adj_list[id]) {
            nodes[id].add_neighbor(v);
        }
    }
}

Graph::~Graph() {
    if(DEBUG) cerr << "Delete Graph" << endl;
}

int Graph::get_node_memory_at(int node_id, int t) {
    return nodes[node_id].get_memory_at(t);
}

int Graph::get_node_memory(int node_id) {
    return nodes[node_id].get_memory();
}

int Graph::get_num_nodes() {
    return num_nodes;
}
int Graph::get_time_limit() {
    return time_limit;
}


double Graph::get_A() { return A; }
double Graph::get_B() { return B; }
double Graph::get_n() { return n; }
double Graph::get_T() { return T; }
double Graph::get_tao() { return tao; }

int Graph::get_succ_request_cnt() {
    return succ_request_cnt;
}

double Graph::get_fidelity_gain() {
    return fidelity_gain;
}

double Graph::get_utilization() {
    return (double)usage / ((double)memory_total * (double)time_limit);
}

int Graph::get_memory_total() {
    return memory_total;
}

void DFS(int x, vector<bool> &vis, vector<int> &par, vector<vector<int>> &adj) {
    vis[x] = true;
    for(auto v : adj[x]) {
        if(!vis[v]) {
            par[v] = x;
            DFS(v, vis, par, adj);
        }
    }
}
vector<int> Graph::get_path(int from, int to) {
    vector<bool> vis(num_nodes, false);
    vector<int> par(num_nodes, -1);
    par[from] = -1;
    DFS(from, vis, par, adj_list);
    vector<int> path;
    int cur = to;
    while(cur != -1) {
        path.push_back(cur);
        cur = par[cur];
    }
    reverse(path.begin(), path.end());

    return path;
}

bool Graph::check_resource(Shape shape) {
    Shape_vector nm = shape.get_node_mem_range();
    for(int i = 0; i < (int)nm.size(); i++) {
        int node = nm[i].first;
        map<int, int> need_amount; // time to amount
        for(pair<int, int> rng : nm[i].second) {
            int left = rng.first, right = rng.second;
            if(left < 0) {
                cerr << "the reserve time is negtive" << endl;
                exit(1);
            }
            if(right >= time_limit) {
                cerr << "the reserve time is exceed the timelimit" << endl;
                cerr << "timelimt = " << time_limit << " reserve time = " << right << endl;
                exit(1);
            }
            for(int t = left; t <= right; t++) {
                need_amount[t]++;
            }
        }
        for(auto P : need_amount) {
            int t = P.first, amount = P.second;
            if(nodes[node].get_memory_at(t) < amount) {
                return false;
            }
        }
    }
    return true;
}
void Graph::reserve_shape(Shape shape) {
    shape.check_valid();
    // cerr << "checked" << endl;
    Shape_vector nm = shape.get_node_mem_range();
    for(int i = 0; i < (int)nm.size(); i++) {
        int node = nm[i].first;
        map<int, int> need_amount; // time to amount
        for(pair<int, int> rng : nm[i].second) {
            int left = rng.first, right = rng.second;

            if(right >= time_limit) {
                cerr << "the reserve time is exceed the timelimit" << endl;
                cerr << "timelimt = " << time_limit << " reserve time = " << right << endl;
                exit(1);
            }
            for(int t = left; t <= right; t++) {
                need_amount[t]++;
            }
        }
        for(auto P : need_amount) {
            int t = P.first, amount = P.second;
            if(nodes[node].get_memory_at(t) < amount) {
                cerr << "node " << node << "\'s memory is not enough at time " << t << endl;
                exit(1);
            }
            usage += amount;
            nodes[node].reserve_memory(t, amount);
        }

        if(!passed_node[node]) {
            passed_node[node] = true;
            memory_total += nodes[node].get_memory();
        }
    }
    for(int i = 1; i < (int)nm.size(); i++) {
        int node1 = nm[i - 1].first;
        int node2 = nm[i].first;
        if(adj_set[node1].count(node2) == 0) {
            cerr << "shape error, the next node is not connected" << endl;
            exit(1);
        } 
    }

    fidelity_gain += shape.get_fidelity(A, B, n, T, tao);
    succ_request_cnt++;
}
