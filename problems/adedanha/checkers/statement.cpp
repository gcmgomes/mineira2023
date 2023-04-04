#include "testlib.h"

using namespace std;

int main(int argc, char* argv[]) {
  registerValidation(argc, argv);

  int n = inf.readInt(1, 500000);
  inf.readEoln();
  for (int i = 0; i < n - 1; i++) {
    int k = inf.readInt(0, 20);
    if (i < n - 1) inf.readSpace();
    else inf.readEoln();
  }
  inf.readEof();
}
