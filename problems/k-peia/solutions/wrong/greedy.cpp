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

const int TH = 5;
const int IT = 100;

ll greedy(vector<pair<int, int>>& w, int c) {
	ll ret = 0;
	for (auto [v, p] : w) if (p <= c) {
		c -= p;
		ret += v;
	}
	return ret;
}

void disturb(vector<pair<int, int>>& w) {
	for (int i = 0; i < w.size(); i++) {
		int l = max(0, i - TH);
		int r = min<int>(w.size() - 1, i + TH);

		int j = l + rand() % (r-l+1);
		swap(w[i], w[j]);
	}
}

ll solve(vector<pair<int, int>>& w, int c) {
	ll ans = 0;
	for (int i = 0; i < IT; i++) {
		disturb(w);
		ans = max(ans, greedy(w, c));
	}
	return ans;
}

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
		vector<pair<int, int>> w;
		for (int i = 0; i < p.size(); i++) w.emplace_back(v[i], p[i]);
		ll at = 0;
		sort(w.begin(), w.end(), [&](auto& a, auto& b) {
			return a.first > b.first;
		});
		at = max(at, solve(w, c));
		sort(w.begin(), w.end(), [&](auto& a, auto& b) {
			return a.first < b.first;
		});
		at = max(at, solve(w, c));
		sort(w.begin(), w.end(), [&](auto& a, auto& b) {
			return a.second < b.second;
		});
		at = max(at, solve(w, c));
		sort(w.begin(), w.end(), [&](auto& a, auto& b) {
			return a.second > b.second;
		});
		at = max(at, solve(w, c));
		sort(w.begin(), w.end(), [&](auto& a, auto& b) {
			return (ll) a.first * b.second > (ll) b.first * a.second;
		});
		at = max(at, solve(w, c));
		sort(w.begin(), w.end(), [&](auto& a, auto& b) {
			return (ll) a.first * b.second > (ll) b.first * a.second;
		});
		at = max(at, solve(w, c));
		for (int i = 0; i < IT; i++) {
			random_whuffle(w.begin(), w.end());
			ans = max(ans, greedy(w, c));
		}
		ans += at;
	}
	cout << ans << endl;
	exit(0);
}

