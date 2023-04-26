#include <iostream>

int R[20], G[20], B[20];

int main() {

    int n;
    std::cin >> n;
    
    for (int i = 0; i < n; i++) {
        std::cin >> R[i] >> G[i] >> B[i];
    }

    int d_ans = -1;
    int r_ans = -1, g_ans = -1, b_ans = -1;
    for (int r = 0; r < 256; r++) {
        for (int g = 0; g < 256; g++) {
            for (int b = 0; b < 256; b++) {
                int d = 1000;

                for (int i = 0; i < n; i++) {
                    int cur_d = abs(r - R[i]) + abs(g - G[i]) + abs(b - B[i]);
                    if (cur_d < d) d = cur_d;
                }

                if (d > d_ans) {
                    d_ans = d;
                    r_ans = r;
                    g_ans = g;
                    b_ans = b;
                }
            }
        }
    }

    std::cout << r_ans << ' ' << g_ans << ' ' << b_ans << std::endl;

    return 0;
}