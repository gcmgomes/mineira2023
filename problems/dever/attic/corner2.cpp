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

	cout << 5 << std::endl;
    cout << "1 2" << std::endl;
    cout << "1 23" << std::endl;
    cout << "1 83" << std::endl;
    cout << "1 677" << std::endl;
    cout << "1 1931" << std::endl;

    return 0;
}
