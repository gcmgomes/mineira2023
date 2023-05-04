#include <iostream>
#include <vector>

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

const int MAX = 5e5+10;

namespace seg {
	int seg[4*MAX], lazy[4*MAX];
	int n, *v;

	ll build(int p=1, int l=0, int r=n-1) {
		lazy[p] = -1;
		if (l == r) return seg[p] = v[l];
		int m = (l+r)/2;
		return seg[p] = build(2*p, l, m) + build(2*p+1, m+1, r);
	}
	void build(int n2, int* v2) {
		n = n2, v = v2;
		build();
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
		return seg[p] = update(a, b, x, 2*p, l, m) + update(a, b, x, 2*p+1, m+1, r);
	}
};

namespace hld {
	vector<int> g[MAX];
	int pos[MAX], sz[MAX];
	int peso[MAX], pai[MAX];
	int h[MAX], v[MAX], t;
	int at[MAX];

	void build_hld(int k, int p = -1, int f = 1) {
		at[t] = k;
		v[pos[k] = t++] = peso[k]; sz[k] = 1;
		for (auto& i : g[k]) if (i != p) {
			pai[i] = k;
			h[i] = (i == g[k][0] ? h[k] : i);
			build_hld(i, k, f); sz[k] += sz[i];

			if (sz[i] > sz[g[k][0]] or g[k][0] == p) swap(i, g[k][0]);
		}
		if (p*f == -1) build_hld(h[k] = k, -1, t = 0);
	}
	void build(int root = 0) {
		t = 0;
		build_hld(root);
		pai[root] = -1;
		seg::build(t, v);
	}
	int query_path(int a, int b) {
		if (pos[a] < pos[b]) swap(a, b);

		if (h[a] == h[b]) return seg::query(pos[b], pos[a]);
		return seg::query(pos[h[a]], pos[a]) + query_path(pai[h[a]], b);
	}
	void update_path(int a, int b, int x) {
		if (pos[a] < pos[b]) swap(a, b);

		if (h[a] == h[b]) return (void)seg::update(pos[b], pos[a], x);
		seg::update(pos[h[a]], pos[a], x); update_path(pai[h[a]], b, x);
	}
	ll query_subtree(int a) {
		return seg::query(pos[a], pos[a]+sz[a]-1);
	}
	void update_subtree(int a, int x) {
		seg::update(pos[a], pos[a]+sz[a]-1, x);
	}
	void sobe(int a) {
		if (a == -1) return;
		if (seg::query(pos[h[a]], pos[h[a]]+sz[h[a]]-1) + pos[a]-pos[h[a]]+1 == sz[h[a]]) {
			seg::update(pos[h[a]], pos[a], 1);
			return sobe(pai[h[a]]);
		}
		int l = pos[h[a]], r = pos[a]+1;
		while (l < r) {
			int m = (l+r)/2;
			if (seg::query(m, m+sz[at[m]]-1) + pos[a]-m+1 == sz[at[m]]) {
				r = m;
			}
			else l = m+1;
		}
		seg::update(l, pos[a], 1);
	}
}

int main() { _
	int n; cin >> n;
	for (int i = 1; i < n; i++) {
		int a, b; cin >> a >> b; a--, b--;
		hld::g[a].push_back(b);
		hld::g[b].push_back(a);
	}
	hld::build(0);
	int q; cin >> q;
	while (q--) {
		int t, a; cin >> t >> a; a--;
		if (t == 1 and !seg::query(hld::pos[a], hld::pos[a])) {
			hld::update_subtree(a, 1);
			hld::sobe(hld::pai[a]);
		}
		if (t == 2) {
			hld::update_subtree(a, 0);
			hld::update_path(0, a, 0);
		}
		if (t == 3) cout << hld::query_subtree(a) << endl;
	}
	exit(0);
}
