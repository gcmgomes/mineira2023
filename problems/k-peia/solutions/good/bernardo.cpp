#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>

using namespace std;

typedef long long ll;

const int MAXK = 100 + 10;
const int MAXN = 500 + 10;
const int MAXT = 500 + 10;

vector<pair<int, int>> peias[MAXN]; // {k, m}
int sapatos[MAXT];

ll memo[MAXN][MAXN*MAXK];

ll solve(int t) {
    int n = peias[t].size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= sapatos[t]; j++) {
            memo[i][j] = -1;
        }
    }

    function<ll(int, int)> dp = [&] (int i, int k) {
        if (i == n) return 0ll;
        ll& ret = memo[i][k];
        if (ret != -1) return ret;

        ret = dp(i + 1, k);
        if (k >= peias[t][i].first)
            ret = max(ret, peias[t][i].second + dp(i + 1, k - peias[t][i].first));

        return ret;
    };

    return dp(0, sapatos[t]);
}

int main() {
    int N, T; cin >> N >> T;
    for (int i = 0; i < N; i++) {
        int k, t, m; cin >> k >> t >> m;
        k /= 2;
        peias[t].emplace_back(k, m);
    }
    for (int i = 1; i <= T; i++) {
        int l, r; cin >> l >> r;
        sapatos[i] = min(l, r);
    }

    ll ans = 0;
    for (int i = 1; i <= T; i++) ans += solve(i);

    cout << ans << endl;

    return 0;
}