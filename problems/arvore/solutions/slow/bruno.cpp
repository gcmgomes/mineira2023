#include <iostream>
#include <vector>

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

const int MAX = 1e5+10;

vector<int> g[MAX];
int on[MAX];
int pai[MAX];

void dfs(int i, int p = -1) {
	for (int j : g[i]) if (j != p) {
		dfs(j, i);
		pai[j] = i;
	}
}

void upd(int i, int x) {
	on[i] = x;
	for (int j : g[i]) if (j != pai[i]) upd(j, x);
}

void fix(int i) {
	for (int j : g[i]) if (j != pai[i]) fix(j);
	int qt = 0, qt_on = 0;
	for (int j : g[i]) if (j != pai[i]) {
		qt++;
		if (on[j]) qt_on++;
	}
	if (!qt) return;
	if (qt == qt_on) on[i] = 1;
	else on[i] = 0;
}

int query(int i) {
	int ret = on[i];
	for (int j : g[i]) if (j != pai[i]) ret += query(j);
	return ret;
}

int main() { _
	int n; cin >> n;
	for (int i = 1; i < n; i++) {
		int a, b; cin >> a >> b; a--, b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}
	dfs(0);
	int q; cin >> q;
	while (q--) {
		int t, a; cin >> t >> a; a--;
		if (t == 1) upd(a, 1);
		if (t == 2) upd(a, 0);
		if (t == 3) cout << query(a) << endl;
		fix(0);
	}
	exit(0);
}
