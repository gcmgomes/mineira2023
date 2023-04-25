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

vector<vector<int>> trans(vector<vector<int>> v) {
	int n = v.size(), m = v[0].size();
	vector ret(m, vector(n, int()));
	for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) ret[j][i] = v[i][j];
	return ret;
}

vector<vector<int>> get_grundy(vector<vector<int>> v) {
	int n = v.size(), m = v[0].size();
	vector<set<int>> baixo(m);
	set<int> all;
	for (int i = 0; i <= n+m; i++) all.insert(i);
	vector<int> tirei;

	for (int i = n-1; i >= 0; i--) {
		for (int j = m-1; j >= 0; j--) {
			if (v[i][j] == -1) {
				baixo[j].clear();
				for (int k : tirei) all.insert(k);
				tirei.clear();
				continue;
			}
			auto it = all.begin();
			for (int k : baixo[j]) if (*it == k) it++;
			int mex = *it;
			v[i][j] = mex;
			all.erase(mex);
			tirei.push_back(mex);
			baixo[j].insert(mex);
		}
		for (int k : tirei) all.insert(k);
		tirei.clear();
	}
	return v;
}

vector<vector<int>> grundy(vector<vector<int>> v) {
	//if (v.size() > v[0].size()) return trans(get_grundy(trans(v)));
	return get_grundy(v);
}

vector<ll> mov(vector<vector<int>> v) {
	v = grundy(v);
	int n = v.size(), m = v[0].size();
	vector<ll> ret(n+m);
	for (int i = 0; i < n; i++) if (v[i][0] >= 0) ret[v[i][0]]++;
	for (int j = 1; j < m; j++) if (v[0][j] >= 0) ret[v[0][j]]++;
	return ret;
}

template<char op, class T> vector<T> FWHT(vector<T> f, bool inv = false) {
	int n = f.size();
	for (int k = 0; (n-1)>>k; k++) for (int i = 0; i < n; i++) if (i>>k&1) {
		int j = i^(1<<k);
		if (op == '^') f[j] += f[i], f[i] = f[j] - 2*f[i];
		if (op == '|') f[i] += (inv ? -1 : 1) * f[j];
		if (op == '&') f[j] += (inv ? -1 : 1) * f[i];
	}
	if (op == '^' and inv) for (auto& i : f) i /= n;
	return f;
}

vector<ll> conv(vector<ll> a, vector<ll> b) {
	int N = max(a.size(), b.size());
	while (N&(N-1)) N++;
	a.resize(N), b.resize(N);
	a = FWHT<'^'>(a), b = FWHT<'^'>(b);
	for (int i = 0; i < a.size(); i++) a[i] = a[i] * b[i];
	return FWHT<'^'>(a, true);
}

int main() { _
	cout << conv(mov(le()), conv(mov(le()), mov(le())))[0] << endl;
	exit(0);
}
