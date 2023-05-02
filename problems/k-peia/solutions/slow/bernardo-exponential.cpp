#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>

using namespace std;

typedef long long ll;

const int MAXN = 500 + 10;
const int MAXT = 500 + 10;

vector<pair<int, int>> peias[MAXN]; // {k, m}
int sapatos[MAXT];

ll solve(int t) {
    int n = peias[t].size();
    ll ret = 0;
    for (ll i = 0; i < (1ll << n); i++) {
        ll win = 0, cost = 0;
        for (int j = 0; j < n; j++) if (i&(1 << j)) {
            cost += peias[t][j].first;
            win += peias[t][j].second;
        }
        if (cost <= sapatos[t]) ret = max(ret, win);
    }
    return ret;
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