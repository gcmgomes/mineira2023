#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;

typedef long long ll;

const int WA_IO = 0;
const int WA_IS = 1;
const int WA_W = 2;

ll ans, ans_p;

int wa(int waCode) {
	printf("NO :/ ");
	switch(waCode) {
		case WA_IO:
			printf("invalid output\n");
			break;
		case WA_IS:
			printf("invalid solution\n");
			break;
		case WA_W:
			printf("path yields wrong value: expected: %lld ; got: %lld\n", ans, ans_p);
			break;
		default:
			printf("\n");
	}
	return 6;
}

int ac() {
	printf("YES!\n");
	return 4;
}

vector<int> read_permutation(FILE* f, int n) {
	vector<int> ret;
	for (int i = 0; i < n; i++) {
		int x;
		if (!fscanf(f, "%d", &x)) exit(wa(WA_IO));
		ret.push_back(x);
	}
	auto ret2 = ret;
	sort(ret2.begin(), ret2.end());
	for (int i = 0; i < n; i++) if (ret2[i] != i+1) exit(wa(WA_IO));
	for (int& i : ret) i--;
	return ret;
}

bool valid(vector<int>& p) {
	int at = p[0];
	int l = at, r = at;
	int cnt = 0;
	while (++cnt < p.size()) {
		if (p[cnt] == l-1) l--;
		else if (p[cnt] == r+1) r++;
		else return false;
	}
	return true;
}

ll compute_ans(vector<pair<int, int>>& v, vector<int>& p) {
	ll ans = 0;
	for (int i = 1; i < p.size(); i++) {
		if (p[i] < p[i-1]) ans += v[p[i-1]].first;
		else ans += v[p[i-1]].second;
	}
	ans += min(v[p.back()].first, v[p.back()].second);
	return ans;
}

int main(int argc, char ** argv) {
	FILE* teamOutputFile = fopen(argv[1], "r");
	FILE* solutionFile = fopen(argv[2], "r");
	FILE* inputFile = fopen(argv[3], "r");

	int n;
	fscanf(inputFile, "%d", &n);
	vector<pair<int, int>> v(n);
	for (int i = 0; i < n; i++) fscanf(inputFile, "%d %d", &v[i].first, &v[i].second);

	vector<int> ans_perm = read_permutation(solutionFile, n);
	ans = compute_ans(v, ans_perm);

	vector<int> p = read_permutation(teamOutputFile, n);
	if (!valid(p)) return wa(WA_IS);
	ans_p = compute_ans(v, p);

	if (ans != ans_p) return wa(WA_W);

	return ac();
}

