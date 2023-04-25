#include <iostream>
#include <cstdlib>
#include <map>
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

    const int n = rnd.next(MIN, MAX);
    cout << n << std::endl;
	std::map<int, char> mp;
	int at = 0;
	for (int i = 0; i < 26; i++) {
		char c = 'a' + i;
		if (c == 'a' or c == 'e' or c == 'i' or c == 'o' or c == 'u' or c == 'y') continue;
		mp[at++] = c;
	}
    for (int i = 0; i < n; ++i) {
		cout << mp[rnd.next(0, 19)];
	}
	cout << std::endl;

    return 0;
}
