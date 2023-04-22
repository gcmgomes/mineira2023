#include<iostream>
#include<vector>
#include<algorithm>

int solve(const std::vector<std::vector<int>>& a) {
    int ans = 0;
    
    for (int i = 0; i < a.size(); i++) {
        int r = 0;
        for (int l = 0; l < a[i].size(); l++) {
            while (r < a[i].size() and (l == r or abs(a[i][r] - a[i][r - 1]) <= 1))
                r++;
            ans = std::max(ans, r - l);
        }
    }

    return ans;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<int>> at(m, std::vector<int>(n));
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cin >> at[j][i];
        }
    }

    std::cout << solve(at) << std::endl;

    return 0;
}