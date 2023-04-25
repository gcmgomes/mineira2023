#include "testlib.h"

using namespace std;

const int MAX_N = 1e5;
const int MAX_AB = 1e5;

int main(int argc, char* argv[]) {
  registerValidation(argc, argv);

  int n = inf.readInt(1, MAX_N);
  inf.readEoln();
  
  for (int i = 0; i < n; i++) {
    inf.readInt(1, MAX_AB);
    inf.readSpace();
    inf.readInt(1, MAX_AB);
    inf.readEoln();
  }
  inf.readEof();
}
