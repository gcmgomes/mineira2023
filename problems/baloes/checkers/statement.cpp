#include "testlib.h"

using namespace std;


int main(int argc, char* argv[]) {
  registerValidation(argc, argv);

  int n = inf.readInt(1, 20);
  inf.readEoln();
  for (int i = 0; i < n; i++) {
    inf.readInt(0, 255);
    inf.readSpace();
    inf.readInt(0, 255);
    inf.readSpace();
    inf.readInt(0, 255);
    inf.readEoln();
  }
  inf.readEof();
}
