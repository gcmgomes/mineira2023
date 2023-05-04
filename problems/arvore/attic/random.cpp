#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>
#include "jngen.h"

using namespace std;

vector<pair<int, int>> from_prufer(vector<int> p) {
	int n = p.size()+2;
	vector<int> d(n, 1);
	for (int i : p) d[i]++;
	p.push_back(n-1);
	int idx, x;
	idx = x = find(d.begin(), d.end(), 1) - d.begin();
	vector<pair<int, int>> ret;
	for (int y : p) {
		ret.push_back({x, y});
		if (--d[y] == 1 and y < idx) x = y;
		else idx = x = find(d.begin()+idx+1, d.end(), 1) - d.begin();
	}
	return ret;
}

vector<pair<int, int>> gen_tree(int n) {
    if (n == 1) return {};
    vector<int> v(n - 2);
    for (int& i : v) i = rnd.next(0, n - 1);
    return from_prufer(v);
}

int main(int argc, char* argv[])
{
    // DO NOT GET RID OF THE registerGen LINE BELOW !!
    //  ... this is required to ensure that test cases are generated deterministically based on
    // the input arguments
    registerGen(argc, argv);
    // ------

    std::ios::sync_with_stdio(false);

    const int n = std::atoi(argv[1]);
    const int q = std::atoi(argv[2]);
    cout << n << endl;

    auto tree = gen_tree(n);
    for (auto [u, v] : tree) cout << u + 1 << ' ' << v + 1 << endl;

    cout << q << endl;

    for (int i = 0; i < q; ++i) {
        cout << rnd.next(1, 3) << " " << rnd.next(1, n) << endl;
    }
    return 0;
}
