#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

typedef long long ll;

const int MAX = 1e5 + 10;
const ll LINF = 0x3f3f3f3f3f3f3f3f;

int n, L[MAX], R[MAX];

pair<ll, vector<int>> ok(int pos) {
    if (pos < 0 or pos >= n) return {LINF, {}};
    vector<int> ans = {pos};
    int l = pos, r = pos;
    ll cost = 0;
    while (true) {
        if (ans.size() == n) {
            if (L[pos] < R[pos]) cost += L[pos], pos = --l;
            else cost += R[pos], pos = ++r;
            break;
        } else {
            if ((L[pos] < R[pos] and l != 0) or r == n - 1) {
                cost += L[pos], pos = --l;
            } else {
                cost += R[pos], pos = ++r;
            }
        }
        ans.push_back(pos);
    }
    if (ans.size() < n) return {LINF, {}};

    return {cost, ans};
}

bool right(int pos) {
    int l = pos, r = pos;
    while (0 <= pos and pos < n) {
        if (L[pos] < R[pos]) pos = --l;
        else pos = ++r;
    }
    return pos == n;
}

int main() {

    cin >> n;
    for (int i = 0; i < n; i++) cin >> L[i] >> R[i];
    // acha o primeiro que cai pra direita
    int l = 0, r = n;
    while (l < r) {
        int m = (l + r)/2;
        right(m) ? r = m : l = m + 1;
    }

    ll ans;

    vector<int> vans = min(ok(l), ok(l - 1)).second;

    for (int i = 0; i < n; i++) {
        cout << vans[i] + 1;
        if (i < n - 1) cout << ' ';
    }
    cout << endl;

    return 0;
}