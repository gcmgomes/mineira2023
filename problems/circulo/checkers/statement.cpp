#include "testlib.h"

using namespace std;

int main(int argc, char* argv[]) {
  registerValidation(argc, argv);

  int n = inf.readInt(1, 3000);
  inf.readEoln();
  
  for (int i = 0; i < n; i++) {
    inf.readInt(1, n);
    if (i < n - 1) inf.readSpace();
    else inf.readEoln();
  }
  inf.readEof();
}
