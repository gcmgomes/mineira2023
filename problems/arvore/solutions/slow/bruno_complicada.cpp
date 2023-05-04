#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

const int MAX = 1e5+10;

enum OP { NOP, OFF, ON };
typedef tuple<int, int, int> T;

T comb(T a, T b) {
	return {min(get<0>(a), get<0>(b)), max(get<1>(a), get<1>(b)), get<2>(a) + get<2>(b)};
}

// seg guarda {min lig, max filhos deslig, soma lig}
namespace seg {
	T seg[4*MAX], off[4*MAX], on[4*MAX];
	int lazy[4*MAX]; // o que eu liguei
	OP lazy2[4*MAX];

	int n, *v;

	void build(int p=1, int l=0, int r=n-1) {
		lazy[p] = 0;
		lazy2[p] = NOP;
		if (l == r) {
			seg[p] = {0, v[l], 0};
			off[p] = {0, v[l], 0};
			on[p] = {1, 0, 1};
			return;
		}
		int m = (l+r)/2;
		build(2*p, l, m), build(2*p+1, m+1, r);
		seg[p] = comb(seg[2*p], seg[2*p+1]);
		off[p] = comb(off[2*p], off[2*p+1]);
		on[p] = comb(on[2*p], on[2*p+1]);
	}
	void build(int n2, int* v2) {
		n = n2, v = v2;
		build();
	}
	void prop(int p, int l, int r) {
		if (l != r) {
			int m = (l+r)/2;
			prop(2*p, l, m);
			prop(2*p+1, m+1, r);
		}

		if (lazy2[p] != NOP) {
			if (lazy2[p] == OFF) seg[p] = off[p];
			if (lazy2[p] == ON) seg[p] = on[p];
			if (l != r) lazy2[2*p] = lazy2[p], lazy2[2*p+1] = lazy2[p];
		} else if (lazy[p]) {
			get<0>(seg[p]) += lazy[p];
			get<1>(seg[p]) -= lazy[p];
			get<2>(seg[p]) += lazy[p] * (r-l+1);
			if (l != r) lazy[2*p] += lazy[p], lazy[2*p+1] += lazy[p];
		}
		lazy[p] = 0;
		lazy2[p] = NOP;
	}
	T query(int a, int b, int p=1, int l=0, int r=n-1) {
		prop(p, l, r);
		if (a <= l and r <= b) return seg[p];
		if (b < l or r < a) return {INF, -INF, 0};
		int m = (l+r)/2;
		return comb(query(a, b, 2*p, l, m), query(a, b, 2*p+1, m+1, r));
	}
	T update(int a, int b, int x, int p=1, int l=0, int r=n-1) { // ligar x (1 ou -1)
		if (a > b) return seg[p];
		prop(p, l, r);
		if (a <= l and r <= b) {
			lazy[p] += x;
			prop(p, l, r);
			return seg[p];
		}
		if (b < l or r < a) return seg[p];
		int m = (l+r)/2;
		return seg[p] = comb(update(a, b, x, 2*p, l, m), update(a, b, x, 2*p+1, m+1, r));
	}
	T update2(int a, int b, OP O, int p=1, int l=0, int r=n-1) { // setar
		if (a > b) return seg[p];
		prop(p, l, r);
		if (a <= l and r <= b) {
			lazy2[p] = O;
			prop(p, l, r);
			return seg[p];
		}
		if (b < l or r < a) return seg[p];
		int m = (l+r)/2;
		return seg[p] = comb(update2(a, b, O, 2*p, l, m), update2(a, b, O, 2*p+1, m+1, r));
	}
	T update3(int a, int x, int p=1, int l=0, int r=n-1) { // += x filhos ligados
		prop(p, l, r);
		if (a < l or r < a) return seg[p];
		if (l == r) {
			get<1>(seg[p]) -= x;
			return seg[p];
		}
		int m = (l+r)/2;
		return seg[p] = comb(update3(a, x, 2*p, l, m), update3(a, x, 2*p+1, m+1, r));
	}
};

namespace hld {
	vector<int> g[MAX];
	int pos[MAX], sz[MAX];
	int peso[MAX], pai[MAX];
	int h[MAX], v[MAX], t;

	void build_hld(int k, int p = -1, int f = 1) {
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
		seg::build(t, v);
		pai[root] = -1;
	}

	void fix(int a, int d, function<bool(T)> ruim) {
		a = pai[a];
		while (a+1) {
			if (ruim(seg::query(pos[h[a]], pos[a]))) {
				int l = pos[h[a]], r = pos[a]+1;
				while (l < r) {
					int m = (l+r)/2;
					if (ruim(seg::query(m, pos[a]))) l = m+1;
					else r = m;
				}
				seg::update(l, pos[a], d);
				seg::update3(l-1, d);
				break;
			}
			seg::update(pos[h[a]], pos[a], d);
			a = pai[h[a]];
		}
	}

	void off_subtree(int a) {
		if (get<2>(seg::query(hld::pos[a], hld::pos[a])) == 1)
			fix(a, -1, [&](T tt) { return get<0>(tt) == 0; });

		seg::update2(pos[a], pos[a]+sz[a]-1, OFF);
	}
	void on_subtree(int a) {
		if (get<2>(seg::query(hld::pos[a], hld::pos[a])) == 0)
			fix(a, 1, [&](T tt) { return get<1>(tt) > 1 or get<2>(tt) > 0; });

		seg::update2(pos[a], pos[a]+sz[a]-1, ON);
	}
	
	int query_subtree(int a) {
		return get<2>(seg::query(pos[a], pos[a]+sz[a]-1));
	}
}

int main() { _
	int n; cin >> n;
	for (int i = 1; i < n; i++) {
		int a, b; cin >> a >> b; a--, b--;
		hld::g[a].push_back(b);
		hld::g[b].push_back(a);
		hld::peso[a]++, hld::peso[b]++;
	}
	for (int i = 1; i < n; i++) hld::peso[i]--;
	hld::build();
	int q; cin >> q;
	while (q--) {
		int t, a; cin >> t >> a; a--;
		if (t == 1) hld::on_subtree(a);
		if (t == 2) hld::off_subtree(a);
		if (t == 3) cout << hld::query_subtree(a) << endl;
	}
	exit(0);
}
