#include <iostream>
#include <vector>

using namespace std;

int LineSweep(vector<int>& line) {
  int best = 1;
  for (int i = 0; i < line.size(); i++) {
    int current = 1;
    for (int j = i + 1; j < line.size(); j++) {
      if (abs(line[j] - line[j - 1]) <= 1) {
        current++;
      } else {
        break;
      }
    }
    if (current > best) {
      best = current;
    }
  }
  return best;
}

int ColSweep(vector<vector<int>>& matriz, int col) {
  int best = 1;
  for (int i = 0; i < matriz.size(); i++) {
    int current = 1;
    for (int j = i + 1; j < matriz.size(); j++) {
      if (abs(matriz[j][col] - matriz[j - 1][col]) <= 1) {
        current++;
      } else {
        break;
      }
    }
    if (current > best) {
      best = current;
    }
  }
  return best;
}

int main() {
  int N = 0, M = 0;
  cin >> N >> M;
  vector<vector<int>> X(N, vector<int>(M));
  for (auto& line : X) {
    for (auto& cell : line) {
      cin >> cell;
    }
  }
  int best = 1;
  for (auto& line : X) {
    best = max(best, LineSweep(line));
  }
  for (int i = 0; i < N; i++) {
    best = max(best, ColSweep(X, i));
  }
  cout << best << endl;
}
