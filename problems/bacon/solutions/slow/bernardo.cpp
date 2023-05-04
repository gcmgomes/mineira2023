#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

const int MAX = 1e5+10;

namespace seg {
    int n, seg[4*MAX], lazy[4*MAX];

    void build(int n2) {
        n = n2;
        memset(lazy, -1, sizeof lazy);
    }
    void prop(int p, int l, int r) {
        if (lazy[p] == -1) return;
        seg[p] = lazy[p]*(r-l+1);
        if (l != r) lazy[2*p] = lazy[p], lazy[2*p+1] = lazy[p];
        lazy[p] = -1;
    }
    int query(int a, int b, int p=1, int l=0, int r=n-1) {
        prop(p, l, r);
        if (a <= l and r <= b) return seg[p];
        if (b < l or r < a) return 0;
        int m = (l+r)/2;
        return query(a, b, 2*p, l, m) + query(a, b, 2*p+1, m+1, r);
    }
    int update(int a, int b, int x, int p=1, int l=0, int r=n-1) {
        prop(p, l, r);
        if (a <= l and r <= b) {
            lazy[p] = x;
            prop(p, l, r);
            return seg[p];
        }
        if (b < l or r < a) return seg[p];
        int m = (l+r)/2;
        return seg[p] = update(a, b, x, 2*p, l, m) +
            update(a, b, x, 2*p+1, m+1, r);
    }
};

int par[MAX], in[MAX], out[MAX], sz[MAX];
std::vector<int> g[MAX];

int dfs(int v, int p, int& t) {
    in[v] = t;
    par[v] = p;
    sz[v] = 1;
    for (int u : g[v]) if (u != p) sz[v] += dfs(u, v, t);
    out[v] = t++;
    
    return sz[v];
}

int main() { _
    
	int n; cin >> n;
	for (int i = 1; i < n; i++) {
		int a, b; cin >> a >> b; a--, b--;
        g[a].push_back(b);
        g[b].push_back(a);
	}
    
    seg::build(n);
    
    {
        int t = 0;
        dfs(0, -1, t);
    }

	int q; cin >> q;
	while (q--) {
		int t, a; cin >> t >> a; a--;
        // seta subarvore pra 1
        if (t == 1) {
            seg::update(in[a], out[a], 1);
            while (par[a] != -1 and seg::query(in[par[a]], out[par[a]]) == sz[par[a]] - 1) {
                a = par[a];
                seg::update(out[a], out[a], 1);
            }
        } else if (t == 2) {
            seg::update(in[a], out[a], 0);
            while (par[a] != -1 and seg::query(in[par[a]], out[par[a]]) < sz[par[a]]) {
                a = par[a];
                seg::update(out[a], out[a], 0);
            }
        } else cout << seg::query(in[a], out[a]) << endl;
	}
	exit(0);
}
