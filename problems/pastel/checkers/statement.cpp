#include "testlib.h"

using namespace std;


void board() {
  long long n = inf.readInt(1, 1e5);
  inf.readSpace();
  long long m = inf.readInt(1, 1e5);
  inf.readEoln();
  
  ensure(n*m <= 1e5);
  
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      char c = inf.readChar();
      ensure(c == '.' or c == 'x');
      if (i == 0 and j == 0) ensure(c == '.');
      if (j == m - 1) inf.readEoln();
    }
  }
}

int main(int argc, char* argv[]) {
  registerValidation(argc, argv);

  board();
  board();
  board();
  inf.readEof();
}
