#include "testlib.h"

using namespace std;


int main(int argc, char* argv[]) {
  registerValidation(argc, argv);

  int n = inf.readInt(1, 1000);
  inf.readSpace();
  int k = inf.readInt(1, 1000);
  inf.readEoln();
  inf.readEof();
}
