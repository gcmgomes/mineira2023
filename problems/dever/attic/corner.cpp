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

    const int MIN = std::atoi(argv[1]);
    const int MAX = std::atoi(argv[2]);

	cout << 2 << std::endl;
    for (int i = 0; i < 2; ++i) cout << "2 2" << std::endl;

    return 0;
}
