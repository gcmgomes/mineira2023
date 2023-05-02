#include "testlib.h"

using namespace std;

int main(int argc, char* argv[]) {
  registerValidation(argc, argv);

  int N = inf.readInt(1, 500);
  inf.readSpace();
  int T = inf.readInt(1, 500);
  inf.readEoln();

  for (int i = 0; i < N; i++) {
    int k = inf.readInt(2, 100);
    inf.readSpace();
    ensure(k%2 == 0);

    inf.readInt(1, T);
    inf.readSpace();

    inf.readInt(1, 1e9);
    inf.readEoln();
  }

  for (int i = 0; i < T; i++) {
    inf.readInt(0, 50'000);
    inf.readSpace();

    inf.readInt(0, 50'000);
    inf.readEoln();
  }
  inf.readEof();
}
