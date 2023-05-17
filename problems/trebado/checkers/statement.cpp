#include "testlib.h"

using namespace std;

bool is_prime(int p) {
  for (int i = 2; i*i < p; i++) if (p%i == 0) return false;
  return true;
}

int main(int argc, char* argv[]) {
  registerValidation(argc, argv);

  int n = inf.readInt(1, 5000);
  inf.readSpace();
  int p = inf.readInt(1e8, 1e9);
  ensure(is_prime(p));
  inf.readEoln();
  inf.readEof();
}
