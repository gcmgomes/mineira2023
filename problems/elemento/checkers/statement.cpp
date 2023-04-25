#include "testlib.h"

using namespace std;

int v[4];

bool ok(int i) {
  for (int j = 0; j < 4; j++) {
    if (i == j) continue;
    if (abs(j - i) != v[j]) return false;
  }
  return true;
}

int main(int argc, char* argv[]) {
  registerValidation(argc, argv);

  for (int i = 0; i < 4; i++) {
    v[i] = inf.readInt(1, 100000);
    if (i < 3) inf.readSpace();
    else inf.readEoln();
  }
  ensure(ok(0) or ok(1) or ok(2) or ok(3) or ok(4));
  inf.readEof();
}
