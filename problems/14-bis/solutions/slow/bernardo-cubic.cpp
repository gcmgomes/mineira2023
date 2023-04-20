#include<iostream>
#include<algorithm>
#include<vector>

int main() {

    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<int>> a(n, std::vector<int>(m));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cin >> a[i][j];
        }
    }

    int ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int j2 = j; j2 < m; j2++) {
                if (j2 != j and abs(a[i][j2] - a[i][j2 - 1]) > 1)
                    break;
                ans = std::max(ans, j2 - j + 1);
            }
        }
    }
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            for (int i2 =i; i2 < n; i2++) {
                if (i2 != i and abs(a[i2][j] - a[i2 - 1][j]) > 1)
                    break;
                ans = std::max(ans, i2 - i + 1);
            }
        }
    }

    std::cout << ans << std::endl;

    return 0;
}