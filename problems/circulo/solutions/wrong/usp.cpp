#include <bits/stdc++.h>
using namespace std;

using ll = long long;
#define int ll
#define all(v) v.begin(),v.end()
#define pb push_back

void dbg_out(){ cerr << endl; }
template<typename H, typename... T>
void dbg_out(H h, T... t) { cerr << ' ' << h; dbg_out(t...); }
#define dbg(...) { cerr << #__VA_ARGS__ << ':'; dbg_out(__VA_ARGS__); }

bool bad(vector<int> v) {
	int n = v.size();

	vector<vector<int>> occ(n);
	for(int i=0;i<n;i++) occ[v[i]].pb(i);
	for(int i=0;i<n;i++) reverse(all(occ[i]));

	for(int k: occ[v[0]]) if(k != 0) {

		int mn = n;

		for(int i=1;i<k;i++) {
			for(int p: occ[v[i]]) if(p > k) {
				mn = min(mn, p);
				if(p > mn) {
					return true;
				}
			}
		}
	}
	return false;
}

void solve() {
	int n; cin >> n;
	vector<int> v(n), f(n);
	for(int &x: v) cin >> x, f[--x]++;

	if(*max_element(all(f)) >= 5) {
		cout << "N" << '\n';
		return;
	}

	for(int i=0;i<n;i++) {
		if(bad(v)) {
			cout << "N" << '\n';
			return;
		}
		rotate(v.begin(), v.begin()+1, v.end());
	}

	cout << "S" << '\n';
}

signed main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	solve();
}

