#pragma GCC optimize("Ofast,no-stack-protector,unroll-loops,fast-math,O3")
#include <iostream>
#include <cmath>
#include <map>
#include <set>
#define TIME_BOUND 1000001
using namespace std;
double A = 0.25, B = 0.75, tao = 0.2, T = 10, n = 2;

double fidelity2time(double fidelity)
{
    if(fidelity <= 0) return TIME_BOUND; 
    return T * pow(-1*log(1/B*fidelity-A/B), 1/n);
}

double time2fidelity(double time)
{
    if(fabs(time - TIME_BOUND) <= 1e-6) return 0;    
    return A + B * exp(-1*pow(time/T, n));
}

double swap(double Fa, double Fb)
{
    // add one time step and run swap
    if(Fa <= 0 || Fb <= 0) return 0;
    double tl = fidelity2time(Fa) + tao;
    double tr = fidelity2time(Fb) + tao;
    double Fl = time2fidelity(tl);
    double Fr = time2fidelity(tr);
    return Fl * Fr + (1.0/3.0) * (1-Fl) * (1-Fr);
}

int generate_new_state(bool l, bool r)
{
    if(l && r) return 4;
    else if(l) return 2;
    else if(r) return 3;
    else return 1;
}

typedef struct dp_key
{
    int t, u, v;
    // 1 denotes u and v both have no memory constraint
    // 2 denotes u has memory constraint
    // 3 denotes v has mamory constraint
    // 4 denotes u and v both have memory constraint
    int state; 
    
    bool operator == (const dp_key& rhs) const
    {
        return t == rhs.t && u == rhs.u && v == rhs.v && state == rhs.state;
    }

    bool operator < (const dp_key& rhs) const 
    {
        if(t != rhs.t) return t < rhs.t;
        else if(u != rhs.u) return u < rhs.u;
        else if(v != rhs.v) return v < rhs.v;
        else if(state != rhs.state) return state < rhs.state;
        else return false;
    }
} dp_key;
int c = 0;
int memory[200][200];
map<dp_key, double> max_fidelity;
map<dp_key, bool> calced;
set<pair<int, int>> lack_memory_set;

double dp(int t, int u, int v, int state)
{
    // run DP to find optimal swapping process (tree) WITH memory constraint

    if(memory[t][u] < 1 || memory[t][v] < 1) return 0; // violate memory constraint
    if(abs(u-v) == 1) return 1; // suppose the fidelity of new link is 1
    if(t < 1) return 0; // violate time constraint
    if(((state == 2) || (state == 4)) && (lack_memory_set.find(make_pair(t, u)) != lack_memory_set.end())) return 0;  // check memory constraint by state
    if(((state == 3) || (state == 4)) && (lack_memory_set.find(make_pair(t, v)) != lack_memory_set.end())) return 0;  // check memory constraint by state

    // won't cut to run DP
    dp_key tmp_key = {t-1, u, v, state};
    if(max_fidelity.find(tmp_key) == max_fidelity.end()) max_fidelity[tmp_key] = dp(t-1, u, v, state);
    double res = max_fidelity[tmp_key];
    res = time2fidelity(fidelity2time(res)+tao);

    // cut in k to run DP
    for(int k = u+1 ; k < v ; k++)
    {
        if(memory[t-1][k] < 2) continue;
        bool l = (state == 2 || state == 4) ? true : false;
        bool r = (state == 3 || state == 4) ? true : false;    
        // left or right subtree which can use k which is in the lack_memory_set
        for(int i = 0 ; i < 2 ; i++)
        {   
            // c++;
            bool which_use_k = (i == 0) ? true : false; // true -> left can not use k ; false -> right can not use k
            dp_key key_left = {t-1, u, k, generate_new_state(l, which_use_k)};
            dp_key key_right = {t-1, k, v, generate_new_state(!which_use_k, r)};
            if(max_fidelity.find(key_left) == max_fidelity.end()) max_fidelity[key_left] = dp(t-1, u, k, generate_new_state(l, which_use_k));
            if(max_fidelity.find(key_right) == max_fidelity.end()) max_fidelity[key_right] = dp(t-1, k, v, generate_new_state(!which_use_k, r));
            double tmp = swap(max_fidelity[key_left], max_fidelity[key_right]);
            if(tmp > res) res = tmp; // update
            // if(u==0 && v==5) cout << t << " " << k << " " << tmp << " " << fidelity2time(tmp) << endl;
        }    
    }
    dp_key key = {t, u, v, state};
    return max_fidelity[key] = res;
}

double alpha[200][200]; // u v
double beta[200][200];  // t v

double dp_separation_oracle(int t, int u, int v)
{
    return 0;
}

int main()
{
    cout << fidelity2time(0.706) << endl;
    cout << time2fidelity(fidelity2time(0.706)) << endl;

    for(int i = 0 ; i < 200 ; i++)   
        for(int j = 0 ; j < 200 ; j++)
            memory[i][j] = 2;

    memory[3][4] = 1;
    // memory[3][3] = 1;
    for(int i = 0 ; i < 200 ; i++)   
        for(int j = 0 ; j < 200 ; j++)
            if(memory[i][j] == 1) 
                lack_memory_set.insert(make_pair(i, j));
    
    cout << dp(64, 0, 64, 1) << endl;
    // cout << c << endl;

    return 0;
}