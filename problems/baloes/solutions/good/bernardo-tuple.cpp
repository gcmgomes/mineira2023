#include <iostream>
#include <vector>
#include <tuple>

int main() {

    int n;
    std::cin >> n;
    
    std::vector<std::tuple<int, int, int>> v(n);
    for (auto& [r, g, b] : v) std::cin >> r >> g >> b;

    int d_ans = -1;
    std::tuple<int, int, int> ans;
    for (int r = 0; r < 256; r++) {
        for (int g = 0; g < 256; g++) {
            for (int b = 0; b < 256; b++) {
                int d = 1000;

                for (int i = 0; i < n; i++) {
                    auto [R, G, B] = v[i];
                    int cur_d = abs(r - R) + abs(g - G) + abs(b - B);
                    if (cur_d < d) d = cur_d;
                }

                if (d > d_ans) d_ans = d, ans = std::tie(r, g, b);
            }
        }
    }

    std::cout << std::get<0>(ans) << ' ' << std::get<1>(ans) << ' ' << std::get<2>(ans) << std::endl;

    return 0;
}