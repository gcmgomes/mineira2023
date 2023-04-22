#include <iostream>
#include <vector>

using namespace std;

int LineSweep(vector<int>& line) {
  int best = 1, current = 1;
  for (int j = 1; j < line.size(); j++) {
    if (abs(line[j] - line[j - 1]) <= 1) {
      current++;
    } else {
      current = 1;
    }
    if (current > best) {
      best = current;
    }
  }
  return best;
}

int main() {
  iostream::sync_with_stdio(false);
  int N = 0, M = 0;
  cin >> N >> M;
  vector<vector<int>> X(N, vector<int>(M)), X_t(M, vector<int>(N));
  for (int i = 0; i < N; i++) {
    auto& line = X[i];
    int j = 0;
    for (auto& cell : line) {
      cin >> cell;
      X_t[j][i] = cell;
      j++;
    }
  }
  int best = 1;
  for (auto& line : X) {
    best = max(best, LineSweep(line));
  }
  for (auto& line : X_t) {
    best = max(best, LineSweep(line));
  }
  cout << best << endl;
}
