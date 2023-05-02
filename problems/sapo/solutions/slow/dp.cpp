#include <iostream>
#include <cstring>
#include <utility>

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

const int MAX = 1e3+10;

ll dp[MAX][MAX][2];
pair<int, int> v[MAX];
int n;

ll solve(int l, int r, int x) {
	if (l < 0 or r == n) return LINF;
	if (l == 0 and r == n-1) {
		if (x == 0) return min(v[0].first, v[0].second);
		return min(v[n-1].first, v[n-1].second);
	}
	auto& ans = dp[l][r][x];
	if (ans+1) return ans;

	int cara = x == 0 ? l : r;
	ans = min(v[cara].first + solve(l-1, r, 0), v[cara].second + solve(l, r+1, 1));
	return ans;
}

void rec(int l, int r, int x) {
	if (l == 0 and r == n-1) {
		return;
	}

	int cara = x == 0 ? l : r;
	if (solve(l, r, x) == v[cara].first + solve(l-1, r, 0)) {
		cout << " " << l-1 + 1;
		rec(l-1, r, 0);
	} else {
		cout << " " << r+1 + 1;
		rec(l, r+1, 1);
	}
	return;
}

int main() { _
	cin >> n;
	for (int i = 0; i < n; i++) cin >> v[i].first >> v[i].second;

	memset(dp, -1, sizeof(dp));
	ll ans = LINF;
	int ini = -1;
	for (int i = 0; i < n; i++) if (solve(i, i, 0) < ans)
		ans = solve(i, i, 0), ini = i;
	cout << ini+1;
	rec(ini, ini, 0);
	cout << endl;
	exit(0);
}
