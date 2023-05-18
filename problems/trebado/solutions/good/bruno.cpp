#include <bits/stdc++.h>

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

ll inv(ll a, ll b) {
	return a > 1 ? b - inv(b%a, a)*b/a : 1;
}

int main() { _
	int k; cin >> k;
	int MOD; cin >> MOD;
	vector<ll> fat(k+1), ifat(k+1);
	fat[0] = 1;
	for (int i = 1; i <= k; i++) fat[i] = i * fat[i-1] % MOD;
	ifat[k] = inv(fat[k], MOD);
	for (int i = k-1; i >= 0; i--) ifat[i] = (i+1) * ifat[i+1] % MOD;

	vector<int> ans(k+1);
	for (int i = 0; i <= k; i++) {
		// resolve problema relaxado pro 1D
		for (int j = 0; j <= i; j++) {
			// j 'ficar parado'
			if ((i-j) % 2) continue;
			int x = (i-j)/2; // x Ls e k Rs

			int cur = fat[i] * ifat[j] % MOD * ifat[x] % MOD * ifat[x] % MOD;
			ans[i] = (ans[i] + cur) % MOD;
		}

		// 2D -> eleva ao quadrado
		ans[i] = (ll) ans[i] * ans[i] % MOD;

		// desrelaxa
		for (int j = 0; j < i; j++) {
			// numero de formas de ficar parado
			int ways = fat[i] * ifat[j] % MOD * ifat[i-j] % MOD;

			ans[i] -= (ll) ans[j] * ways % MOD;
			if (ans[i] < 0) ans[i] += MOD;
		}

	}
	cout << ans[k] << endl;
	exit(0);
}
