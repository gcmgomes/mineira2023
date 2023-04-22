#include<iostream>
#include<algorithm>
#include<vector>

#define _ std::ios_base::sync_with_stdio(0);std::cin.tie(0);

int main() { _

    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<int>> a(n, std::vector<int>(m));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cin >> a[i][j];
        }
    }

    int ans = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int j2 = j + 1; j2 < m; j2++) {
                if (abs(a[i][j2] - a[i][j2 - 1]) > 1)
                    break;
                ans = std::max(ans, j2 - j + 1);
            }
        }
    }
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            for (int i2 = i + 1; i2 < n; i2++) {
                if (abs(a[i2][j] - a[i2 - 1][j]) > 1)
                    break;
                ans = std::max(ans, i2 - i + 1);
            }
        }
    }

    std::cout << ans << std::endl;

    return 0;
}