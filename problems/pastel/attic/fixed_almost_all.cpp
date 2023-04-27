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

    const int n = std::atoi(argv[1]);
    const int m = std::atoi(argv[2]);

	for (int t = 0; t < 3; t++) {
		cout << n << " " << m << std::endl;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (i+j == 0) cout << '.';
				else {
					if (rnd.next(0, 10000) == 0) cout << 'x';
					else cout << '.';
				}
			}
			cout << std::endl;
		}
	}

    return 0;
}
