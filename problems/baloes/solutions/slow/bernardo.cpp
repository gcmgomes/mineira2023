#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

std::vector<std::tuple<int, int, int>> all, in;

int get_dist(std::tuple<int, int, int> t) {
    int d = 1000;
    auto [r, g, b] = t;
    for (auto [r2, g2, b2] : in) d = std::min(d, abs(r - r2) + abs(g - g2) + abs(b - b2));
    return d;
}

bool ok(int d) {
    for (auto t : all) if (get_dist(t) >= d) return true;
    return false;
}

int main() {

    int n; std::cin >> n;
    in.resize(n);
    for (int i = 0; i < n; i++) {
        auto& [r, g, b] = in[i];
        std::cin >> r >> g >> b;
    }

    for (int r = 0; r < 256; r++)
        for (int g = 0; g < 256; g++)
            for (int b = 0; b < 256; b++)
                all.emplace_back(r, g, b);

    int l = 0, r = 800;
    while (l < r) {
        int m = (l + r + 1)/2;

        ok(m) ? l = m : r = m - 1;
    }

    std::tuple<int, int, int> ans(256, 256, 256);
    for (auto t : all) {
        if (get_dist(t) == l) 
            ans = std::min(ans, t);
    }
    
    std::cout << std::get<0>(ans) << ' ' << std::get<1>(ans) << ' ' << std::get<2>(ans) << std::endl;

    return 0;
}