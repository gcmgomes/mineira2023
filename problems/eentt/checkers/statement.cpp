#include "testlib.h"

using namespace std;

const int MAX_N = 1e6;

int main(int argc, char* argv[]) {
  registerValidation(argc, argv);

  int n = inf.readInt(1, MAX_N);
  inf.readEoln();
  
  for (int i = 0; i < n; i++) {
    char c = inf.readChar();
    ensure('a' <= c and c <= 'z');
    ensure(c != 'a' and c != 'e' and c != 'i' and c != 'o' and c != 'u' and c != 'y');
  }
  inf.readEoln();
  inf.readEof();
}
