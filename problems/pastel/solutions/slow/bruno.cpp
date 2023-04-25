#include <bits/stdc++.h>

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

vector<vector<int>> le() {
	int n, m; cin >> n >> m;
	vector ret(n, vector(m, int()));
	for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) {
		char c; cin >> c;
		if (c == 'x') ret[i][j] = -1;
	}
	return ret;
}

vector<vector<int>> grundy(vector<vector<int>> v) {
	int n = v.size(), m = v[0].size();

	for (int i = n-1; i >= 0; i--) {
		for (int j = m-1; j >= 0; j--) {
			if (v[i][j] == -1) {
				continue;
			}
			vector<int> qt(n+m);
			for (int ii = i+1; ii < n; ii++) {
				if (v[ii][j] == -1) break;
				qt[v[ii][j]] = 1;
			}
			for (int jj = j+1; jj < m; jj++) {
				if (v[i][jj] == -1) break;
				qt[v[i][jj]] = 1;
			}
			int mex = 0;
			while (qt[mex]) mex++;
			v[i][j] = mex;
		}
	}
	return v;
}

vector<int> mov(vector<vector<int>> v) {
	v = grundy(v);
	int n = v.size(), m = v[0].size();
	vector<int> ret(n+m);
	for (int i = 0; i < n; i++) if (v[i][0] >= 0) ret[v[i][0]]++;
	for (int j = 1; j < m; j++) if (v[0][j] >= 0) ret[v[0][j]]++;
	return ret;
}

ll conv(vector<int> a, vector<int> b, vector<int> c) {
	ll ans = 0;
	for (int i = 0; i < a.size(); i++)
		for (int j = 0; j < b.size(); j++)
			for (int k = 0; k < c.size(); k++)
				if ((i^j^k) == 0) ans += (ll) a[i] * b[j] * c[k];
	return ans;
}

int main() { _
	cout << conv(mov(le()), mov(le()), mov(le())) << endl;
	exit(0);
}
