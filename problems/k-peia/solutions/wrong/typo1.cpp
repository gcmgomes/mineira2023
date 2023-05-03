#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

int main() { _
	int n, T; cin >> n >> T;
	vector<tuple<vector<int>, vector<int>, int>> moch(T); // {valor, peso, cap}
	for (int i = 0; i < n; i++) {
		int k, t, m; cin >> k >> t >> m;
		get<0>(moch[t-1]).push_back(m);
		get<1>(moch[t-1]).push_back(k/2);
	}
	for (int i = 0; i < T; i++) {
		int l, r; cin >> l >> r;
		get<2>(moch[i]) = min(l, r);
	}
	ll ans = 0;
	for (auto& [v, p, c] : moch) {
		vector<ll> dp(c);
		for (int i = 0; i < v.size(); i++)
			for (int j = c-1; j >= p[i]; j--) dp[j] = max(dp[j], v[i] + dp[j - p[i]]);
		ans += dp.back();
	}
	cout << ans << endl;
	exit(0);
}

