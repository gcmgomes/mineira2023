#include "testlib.h"

using namespace std;


int main(int argc, char* argv[]) {
  registerValidation(argc, argv);

  int n = inf.readInt(1, 2e5);
  inf.readEoln();
  for (int i = 0; i < n; i++) {
    int a = inf.readInt(1, 3);
    inf.readSpace();
    int b = inf.readInt(1, 3);
    inf.readEoln();
	ensure(a != b);
  }
  inf.readEof();
}
