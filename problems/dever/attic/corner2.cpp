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

	cout << 4 << std::endl;
    cout << "1 5" << std::endl;
    cout << "1 13" << std::endl;
    cout << "1 1783" << std::endl;
    cout << "1 17257" << std::endl;

    return 0;
}
