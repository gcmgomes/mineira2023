#include <bits/stdc++.h>
using namespace std;

const int WA_I = 0;
const int WA_D = 1;

int wa(int waCode) {
	printf("NO :/ ");
	switch(waCode) {
		case WA_I:
			printf("invalid float\n");
			break;
		case WA_D:
			printf("values differ by more than 10^-6\n");
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

double read_double(FILE* f) {
	double d;
	if (!fscanf(f, "%lf", &d)) {
		exit(wa(0));
	}
	return d;
}

int main(int argc, char ** argv) {
	FILE* teamOutputFile = fopen(argv[1], "r");
	FILE* solutionFile = fopen(argv[2], "r");
	FILE* inputFile = fopen(argv[3], "r");

	double d1 = read_double(solutionFile);
	double d2 = read_double(teamOutputFile);

	if (abs(d1 - d2) > 1e-2) return wa(1);
	return ac();
}

