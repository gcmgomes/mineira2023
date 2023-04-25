#include <iostream>
#include <vector>

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

int main() { _
	int n; cin >> n;
	string s; cin >> s;
	ll ans = 0;
	for (int i = 0; i < n; i++) {
		vector<int> qt(26);
		int uns = 0;
		for (int j = i; j < n; j++) {
			int val = qt[s[j] - 'a'] ^= 1;
			if (val == 1) uns++;
			else uns--;

			if (uns <= 1) ans++;
		}
	}
	cout << ans << endl;
	return 0;
}
