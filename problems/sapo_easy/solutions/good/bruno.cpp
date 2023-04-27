#include <iostream>
#include <vector>

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

pair<bool, vector<int>> solve(string& s, int at) {
	vector<int> ret;
	int l = at, r = at;
	while (at >= 0 and at < s.size()) {
		ret.push_back(at);
		if (s[at] == 'L') at = --l;
		else at = ++r;
	}
	return {ret.size() == s.size(), ret};
}

int main() { _
	int n; cin >> n;
	string s; cin >> s;
	int x = 0;
	for (char c : s) if (c == 'L') x++;
	for (int i = max(0, x - 1); i <= x; i++) {
		auto [deu, ans] = solve(s, i);
		if (deu) {
			for (int j : ans) cout << j+1 << " ";
			cout << endl;
			return 0;
		}
	}
	cout << -1 << endl;
	exit(0);
}
