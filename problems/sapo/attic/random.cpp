#include <iostream>
#include <cstdlib>
#include "jngen.h"

int main(int argc, char* argv[])
{
    // DO NOT GET RID OF THE registerGen LINE BELOW !!
    //  ... this is required to ensure that test cases are generated deterministically based on
    // the input arguments
    registerGen(argc, argv);
    // ------

    std::ios::sync_with_stdio(false);
    using std::cin;
    using std::cout;

    const int LIM1 = std::atoi(argv[1]);
    const int LIM2 = std::atoi(argv[2]);

    const int n = rnd.next(1, LIM1);
    cout << n << std::endl;
    for (int i = 0; i < n; ++i) cout << rnd.next(1, LIM2) << " " << rnd.next(1, LIM2) << std::endl;

    return 0;
}
