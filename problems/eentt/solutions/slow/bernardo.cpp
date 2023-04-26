#include <iostream>
#include <string>

using namespace std;

int pref[1 << 26];

int main() {

    int n; cin >> n;
    string s; cin >> s;

    pref[0] = 1;
    int mask = 0;
    long long ans = 0;
    for (int i = 0; i < n; i++) {
        mask ^= 1 << (s[i] - 'a');
        ans += pref[mask];

        for (int j = 0; j < 26; j++)
            ans += pref[mask ^ (1 << j)];

        pref[mask]++;
    }
    cout << ans << endl;

    return 0;
}