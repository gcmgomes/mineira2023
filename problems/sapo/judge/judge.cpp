#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int WA_I = 0;
const int WA_D = 1;
const int WA_N = 2;
const int WA_S = 3;

int wa(int waCode) {
	printf("NO :/ ");
	switch(waCode) {
		case WA_I:
			printf("invalid output\n");
			break;
		case WA_D:
			printf("invalid solution\n");
			break;
		case WA_N:
			printf("competitor does not have solution but the judge does\n");
			break;
		case WA_S:
			printf("competitor has solution but the judge does not\n");
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
		if (!fscanf(f, "%d", &x)) exit(wa(0));
		if (i == 0 and x == -1) exit(wa(2));
		ret.push_back(x);
	}
	auto ret2 = ret;
	sort(ret2.begin(), ret2.end());
	for (int i = 0; i < n; i++) if (ret2[i] != i+1) exit(wa(0));
	return ret;
}

bool check(int n, string s, vector<int> v) {
	for (int& i : v) i--;
	int at = v[0];
	int l = at, r = at;
	int cnt = 0;
	while (cnt < n) {
		if (at != v[cnt++]) return false;
		if (s[at] == 'L') at = --l;
		else at = ++r;
	}
	return true;
}

const int MAX = 1e6+10;

char S[MAX];

int main(int argc, char ** argv) {
	FILE* teamOutputFile = fopen(argv[1], "r");
	FILE* solutionFile = fopen(argv[2], "r");
	FILE* inputFile = fopen(argv[3], "r");

	int n;
	fscanf(inputFile, "%d", &n);
	fscanf(inputFile, "%s", S);
	string s(S);

	int x;
	fscanf(solutionFile, "%d", &x);
	if (x == -1) {
		int xx;
		if (!fscanf(teamOutputFile, "%d", &xx)) return wa(0);
		if (xx != -1) return wa(3);
		return ac();
	}

	vector<int> v = read_permutation(teamOutputFile, n);

	if (check(n, s, v)) return ac();
	return wa(1);
}

