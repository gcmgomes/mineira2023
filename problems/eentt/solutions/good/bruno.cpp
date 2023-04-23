#include <bits/stdc++.h>

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

const int MAX = (1<<20) + 10;

int qt[MAX];
int val[26];

int main() { _
	int at = 0;
	for (char c = 'a'; c <= 'z'; c++) {
		if (c == 'a' or c == 'e' or c == 'i' or c == 'o' or c == 'u' or c == 'y') continue;
		val[c-'a'] = at++;
	}

	int n; cin >> n;
	string s; cin >> s;
	qt[0] = 1;
	int mask = 0;
	ll ans = 0;
	for (int i = 0; i < n; i++) {
		mask ^= (1<<val[s[i]-'a']);
		ans += qt[mask];
		for (int j = 0; j < 20; j++) ans += qt[mask^(1<<j)];
		qt[mask]++;
	}
	cout << ans << endl;
	return 0;
}
