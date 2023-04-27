#include <iostream>

int v[4];

int main() {
    int n;
    std::cin >> n;
    v[2] = 1;

    for (int i = 0; i < n; i++) {
        int a, b;
        std::cin >> a >> b;
        std::swap(v[a], v[b]);
    }

    for (int i = 1; i <= 3; i++) {
        if (v[i]) {
            std::cout << i << std::endl;
        }
    }

    return 0;
}