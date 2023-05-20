#include "testlib.h"

using namespace std;


int main(int argc, char* argv[]) {
  registerValidation(argc, argv);

  int n = inf.readInt(1, 200000);
  inf.readEoln();
  
  for (int i = 0; i < n - 1; i++) {
    inf.readInt(0, 20);
    inf.readEoln();
  }
  inf.readEof();
}
