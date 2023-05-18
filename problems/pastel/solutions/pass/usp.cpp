#include <bits/stdc++.h>
using namespace std;

using ll = long long;
#define int ll

void dbg_out(){ cerr << endl; }
template<typename H, typename... T>
void dbg_out(H h, T... t) { cerr << ' ' << h; dbg_out(t...); }
#define dbg(...) { cerr << #__VA_ARGS__ << ':'; dbg_out(__VA_ARGS__); }

const ll N = 1<<20;

using T = int;
void fwht(T io[], ll n) {
	for(ll d = 1; d < n; d <<=1) {
		for(ll i = 0, m = d<<1; i<n;i+=m) {
			for(ll j = 0; j < d; j++) {
				T x = io[i+j], y = io[i+j+d];
				io[i+j] = (x+y), io[i+j+d] = (x-y);
			}
		}
	}
}

void ufwht(T io[], ll n) {
	for(ll d=1;d<n;d<<=1) {
		for(ll i=0,m=d<<1;i<n;i+=m) {
			for(ll j=0;j<d;j++) {
				T x = io[i+j], y = io[i+j+d];
				io[i+j] = (x+y)>>1, io[i+j+d] = (x-y) >> 1;
			}
		}
	}
}

void convolution(T a[], T b[], ll n) {
	fwht(a, n), fwht(b, n);
	for(ll i=0;i<n;i++)
		a[i] = a[i]*b[i];
	ufwht(a, n);
}

void self_convolution(T a[], ll n) {
	fwht(a, n);
	for(ll i=0;i<n;i++)
		a[i] = a[i] * a[i];
	ufwht(a, n);
}

vector<int> solve() {
	int n, m; cin >> n >> m;
	vector p(n, vector<int>(m));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			char c; cin >> c;
			if (c == '.') p[i][j] = 0;
			else p[i][j] = 1;
		}
	}
	if (n > m) {
		vector pp(m, vector<int>(n));
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				pp[j][i] = p[i][j];
		p = pp;
		swap(n, m);
	}
	vector ma(n, vector<int>(m));
	for (int i = n - 1; i >= 0; i--) {
		int mex = 0;
		vector<int> freq(n * m + 50);
		for (int j = m - 1; j >= 0; j--) {
			if (p[i][j] == 1) {
				for (int jj = j + 1; jj < m && p[i][jj] == 0; jj++) {
					freq[ma[i][jj]]--;
				}
				mex = 0;
				ma[i][j] = n * m + 1;
				continue;
			}
			for (int ii = i + 1; ii < n && p[ii][j] == 0; ii++) {
				freq[ma[ii][j]]++;
			}
			int mexl = mex;
			while (freq[mexl] != 0) mexl++;
			ma[i][j] = mexl;
			freq[mexl]++;
			for (int ii = i + 1; ii < n && p[ii][j] == 0; ii++) {
				freq[ma[ii][j]]--;
			}
			while (freq[mex] != 0) mex++;
		}
	}
	auto freq = vector<int>(n * m + 50, 0);
	for (int i = 0; i < n && !p[i][0]; i++) {
		freq[ma[i][0]]++;
	}
	for (int j = 0; j < m && !p[0][j]; j++) {
		freq[ma[0][j]]++;
	}
	freq[ma[0][0]]--;

	return freq;
}

signed main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	auto f1 = solve();
	auto f2 = solve();
	auto f3 = solve();
	f1.resize(N);
	f2.resize(N);
	f3.resize(N);
	convolution(f1.data(), f2.data(), N);
	convolution(f1.data(), f3.data(), N);
	auto ans = f1;
	cout << f1[0] << endl;
}

