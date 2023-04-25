#include <bits/stdc++.h>
 
using namespace std;
 
#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'
 
typedef long long ll;
 
const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;
 
#define MAX 1010
struct sat {
    int n, tot;
    vector<vector<int>> g;
    vector<int> vis, comp, id, ans;
    stack<int> s;
 
    sat() {}
    sat(int n_) : n(n_), tot(n), g(2*n) {}
 
    int dfs(int i, int& t) {
        int lo = id[i] = t++;
        s.push(i), vis[i] = 2;
        for (int j : g[i]) {
            if (!vis[j]) lo = min(lo, dfs(j, t));
            else if (vis[j] == 2) lo = min(lo, id[j]);
        }
        if (lo == id[i]) while (1) {
            int u = s.top(); s.pop();
            vis[u] = 1, comp[u] = i;
            if ((u>>1) < n and ans[u>>1] == -1) ans[u>>1] = ~u&1;
            if (u == i) break;
        }
        return lo;
    }
 
    void add_impl(int x, int y) { // x -> y = !x ou y
        x = x >= 0 ? 2*x : -2*x-1;
        y = y >= 0 ? 2*y : -2*y-1;
        g[x].push_back(y);
        g[y^1].push_back(x^1);
    }
    void add_cl(int x, int y) { // x ou y
        add_impl(~x, y);
    }
    void add_xor(int x, int y) { // x xor y
        add_cl(x, y), add_cl(~x, ~y);
    }
    void add_eq(int x, int y) { // x = y
        add_xor(~x, y);
    }
    void add_true(int x) { // x = T
        add_impl(~x, x);
    }
    void at_most_one(vector<int> v) { // no max um verdadeiro
        g.resize(2*(tot+v.size()));
        for (int i = 0; i < v.size(); i++) {
            add_impl(tot+i, ~v[i]);
            if (i) {
                add_impl(tot+i, tot+i-1);
                add_impl(v[i], tot+i-1);
            }
        }
        tot += v.size();
    }
 
    pair<bool, vector<int>> solve() {
        ans = vector<int>(n, -1);
        int t = 0;
        vis = comp = id = vector<int>(2*tot, 0);
        for (int i = 0; i < 2*tot; i++) if (!vis[i]) dfs(i, t);
        for (int i = 0; i < tot; i++)
            if (comp[2*i] == comp[2*i+1]) return {false, {}};
        return {true, ans};
    }
};
 
bool v[MAX];
int cnt[MAX];
int c[MAX];
int main() { _
    int n;
    cin >> n;
    bool valid = true;
    for(int i = 0; i < n; i++) {
        cin >> c[i];
        cnt[c[i]]++;
        if(cnt[c[i]] > 4) valid = false;
    }
    if(!valid) {
        cout << "N" << endl;
        exit(0);
    }
    vector<pair<int, int>> v;
    for(int i = 0; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            if(c[i] == c[j]) v.push_back({i, j});
        }
    }
    sat s(v.size());
    for(int i = 0; i < v.size(); i++) {
        for(int j = i + 1; j < v.size(); j++) {
            auto a = v[i];
            auto b = v[j];
            if(a > b) swap(a, b);
            if(a.first == b.first || a.first == b.second) continue;
            if(a.second <= b.first || a.second >= b.second) continue;
            s.add_xor(i, j);
        }
    }
    valid = s.solve().first;
    cout << (valid ? "S" : "N") << endl;
    exit(0);
}
 
