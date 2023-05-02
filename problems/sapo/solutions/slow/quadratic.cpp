#include <iostream>
#include <vector>
#include <utility>

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

pair<ll, vector<int>> solve(vector<pair<int, int>>& v, int at) {
	vector<int> ret;
	ll val = 0;
	int l = at, r = at;
	while (true) {
		ret.push_back(at);
		if (r+1 == v.size() and l == 0) {
			val += min(v[at].first, v[at].second);
			break;
		}
		if (r+1 == v.size()) {
			val += v[at].first;
			at = --l;
			continue;
		}
		if (l == 0) {
			val += v[at].second;
			at = ++r;
			continue;
		}
		if (v[at].first < v[at].second) {
			val += v[at].first;
			at = --l;
		} else {
			val += v[at].second;
			at = ++r;
		}
	}
	return {val, ret};
}

int main() { _
	int n; cin >> n;
	vector<pair<int, int>> v(n);
	for (auto& [a, b] : v) cin >> a >> b;
	int x = 0;
	for (int i = 0; i < n; i++) if (v[i].first < v[i].second) x++;
	vector<int> ans;
	ll val = LINF;
	for (int i = 0; i < n; i++) {
		auto [val2, ans2] = solve(v, i);
		if (val2 < val) val = val2, ans = ans2;
	}
	for (int i = 0; i < n; i++) {
		if (i) cout << " ";
		cout << ans[i]+1;
	}
	cout << endl;
	exit(0);
}
