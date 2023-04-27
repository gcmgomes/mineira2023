#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

string s;

vector<int> ok(int pos) {
    vector<bool> vis(s.size());
    vector<int> ans;
    int L = pos, R = pos;
    while (0 <= pos and pos < s.size()) {
        vis[pos] = true;
        ans.push_back(pos);
        pos = s[pos] == 'L' ? --L : ++R;
    }

    return find(begin(vis), end(vis), false) == end(vis) ? ans : vector<int>();
}

bool right(int pos) {
    int L = pos, R = pos;
    while (0 <= pos and pos < s.size()) pos = s[pos] == 'L' ? --L : ++R;

    return pos == s.size();
}

int main() {

    int n; cin >> n >> s;

    // acha o primeiro que cai pra direita
    int l = 0, r = s.size();
    while (l < r) {
        int m = (l + r)/2;
    
        right(m) ? r = m : l = m + 1;
    }

    vector<int> ans = ok(l);
    if (not ans.empty()) {
        for (int i : ans) cout << i + 1 << ' ';
        cout << endl;
        return 0;
    } 
    ans = ok(l - 1);
    if (not ans.empty()) {
        for (int i : ans) cout << i + 1 << ' ';
        cout << endl;
        return 0;
    } 

    cout << -1 << endl;

    return 0;
}