#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

const int MAX = 1e5+10;
const int MOD = 998244353;

vector<int> fact(int n) {
	vector<int> ret;
	for (int i = 2; i < n/i; i++) if (n%i == 0) {
		ret.push_back(i);
		while (n%i == 0) n /= i;
	}
	if (n > 1) ret.push_back(n);
	return ret;
}

int tem[MAX];

int main() { _
	int n; cin >> n;
	while (n--) {
		int a, b; cin >> a >> b;
		for (int i : fact(b / gcd(a, b))) tem[i] = 1;
	}
	if (accumulate(tem, tem + MAX, 0ll) == 0) return cout << 2 << endl, 0;
	ll ans = 1;
	for (int i = 0; i < MAX; i++) if (tem[i]) ans = ans * i % MOD;
	cout << ans << endl;
	exit(0);
}
