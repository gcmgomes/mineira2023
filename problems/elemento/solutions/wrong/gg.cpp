#include <iostream>
#include <vector>

using namespace std;

int main() {
  vector<int> v(4);
  for (auto& x : v) {
    cin >> x;
  }
  if (v[1] == 1 && v[2] == 2 && v[3] == 3) cout << 0;
  if (v[0] == 1 && v[2] == 1 && v[3] == 2) cout << 1;
  if (v[0] == 2 && v[1] == 1 && v[3] == 1) cout << 2;
  if (v[0] == 3 && v[1] == 2 && v[2] == 1) cout << 3;

  cout << endl;
  return 0;
}
