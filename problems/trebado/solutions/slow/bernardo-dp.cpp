#include <iostream>

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

const int MAX = 500;
const int MOD = 1e9 + 7;

int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[8] = {-1,  0,  1,-1, 1,-1, 0, 1};

int memo[MAX][2*MAX + 10][2*MAX + 10];

int dp(int left, int x, int y) {
	if (left == 0) return x == 0 and y == 0;
	auto& ans = memo[left][x + MAX][y + MAX];
	if (ans != -1) return ans;

	ans = 0;
	for (int i = 0; i < 8; i++) {
		ans = (ans + dp(left - 1, x + dx[i], y + dy[i]))%MOD;
	}
	return ans;
}

int main() { _
	memset(memo, -1, sizeof memo);
	int k; cin >> k;

	cout << dp(k, 0, 0) << endl;

	exit(0);
}
