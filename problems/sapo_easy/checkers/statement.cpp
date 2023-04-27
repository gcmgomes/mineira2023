#include "testlib.h"

using namespace std;

int main(int argc, char* argv[]) {
  registerValidation(argc, argv);

  long long n = inf.readInt(1, 1e6);
  inf.readEoln();
  
  for (int i = 0; i < n; i++) {
    char c = inf.readChar();
    ensure(c == 'L' or c == 'R');
  }
  inf.readEoln();
  inf.readEof();
}
