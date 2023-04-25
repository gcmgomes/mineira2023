#include "testlib.h"

using namespace std;


int main(int argc, char* argv[]) {
  registerValidation(argc, argv);

  int n = inf.readInt(1, 200000);
  inf.readSpace();
  int m = inf.readInt(1, 200000);
  inf.readEoln();
  ensure(n*m <= 2e5);
  
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
        inf.readInt(0, 1000000000);
        if (j < m - 1) inf.readSpace();
        else inf.readEoln();
    }
  }
  inf.readEof();
}
