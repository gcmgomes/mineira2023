#include <iostream>

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

const int MOD = 1e9 + 7, MAX = 5e3 + 10;

ll fat[MAX], ifat[MAX];

ll inv(ll a, ll b) {
	return a > 1 ? b - inv(b%a, a)*b/a : 1;
}

void build() {
	fat[0] = ifat[0] = 1;
	for (ll i = 1; i < MAX; i++) fat[i] = i*fat[i - 1]%MOD;
	for (ll i = 1; i < MAX; i++) ifat[i] = inv(fat[i], MOD);
}

ll choose(int n, int m) {
	if (n < m or m < 0) return 0;
	return fat[n]*ifat[m]%MOD*ifat[n - m]%MOD;
}

int main() { _
	build();

	int k; cin >> k;

	ll ans = 0;
	for (int n = 0; n <= k; n++) {
		for (int l = 0; l <= k; l++) {
			ll cur = choose(k, 2*n)*choose(2*n, n)%MOD;
			int over = 2*l - (k - 2*n);
			cur = cur*choose(2*n, over)%MOD*choose(2*l, l)%MOD;
			ans = (ans + cur)%MOD;
		}
	}

	cout << ans << endl;

	exit(0);
}
