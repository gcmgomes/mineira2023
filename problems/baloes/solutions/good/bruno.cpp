#include <iostream>
#include <tuple>
#include <vector>
#include <algorithm>

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

int main() { _
	int n; cin >> n;
	tuple<int, int, int> ans(INF, INF, INF);
	int val = -1;
	vector<tuple<int, int, int>> v(n);
	for (auto& [a, b, c] : v) cin >> a >> b >> c;
	for (int x = 0; x < 256; x++)
		for (int y = 0; y < 256; y++)
			for (int z = 0; z < 256; z++) {
				int at = INF;
				for (auto [a, b, c] : v)
					at = min(at, abs(a-x) + abs(b-y) + abs(c-z));
				if (at > val)
					val = at, ans = {x, y, z};
			}
	cout << get<0>(ans) << " " << get<1>(ans) << " " << get<2>(ans) << endl;
	exit(0);
}
