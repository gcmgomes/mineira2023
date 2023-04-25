#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

const int MAX = 2e3+10;

vector<int> g[MAX];

bool inter(tuple<int, int, int> a, tuple<int, int, int> b) {
	if (get<0>(b) < get<0>(a)) swap(a, b);
	if (get<0>(a) == get<0>(b) or get<1>(a) == get<0>(b)) return false;
	return get<0>(b) < get<1>(a) and get<1>(a) < get<1>(b);
}

bool bip;
int c[MAX];

void dfs(int i, int cc = 1) {
	c[i] = cc;
	for (int j : g[i]) {
		if (!c[j]) dfs(j, 3 - cc);
		else if (c[j] == c[i]) bip = false;
	}
}

int main() { _
	int n; cin >> n;
	vector<vector<int>> pos(n);
	for (int i = 0; i < n; i++) {
		int cor; cin >> cor; cor--;
		pos[cor].push_back(i);
	}
	vector<tuple<int, int, int>> par;
	for (int i = 0; i < n; i++) {
		//if (pos[i].size() > 4) return cout << "N" << endl, 0;
		for (int j = 0; j < pos[i].size(); j++) for (int k = j+1; k < pos[i].size(); k++)
			par.emplace_back(pos[i][j], pos[i][k], i);
	}
	for (int i = 0; i < par.size(); i++) for (int j = i+1; j < par.size(); j++)
		if (inter(par[i], par[j])) g[i].push_back(j), g[j].push_back(i);
	bip = true;
	for (int i = 0; i < par.size(); i++) if (!c[i]) dfs(i);
	if (bip) cout << "S" << endl;
	else cout << "N" << endl;
	exit(0);
}
