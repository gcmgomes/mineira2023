#include <iostream>
#include <vector>

int main() {
  int N, S = 0;

  std::cin >> N;
  for (int i = 0; i < N - 1; i++) {
    int dedos;
    std::cin >> dedos;
    S += dedos;
  }
  
  std::vector<int> v(N, -1);
  for (int dedos_adriana = 20; dedos_adriana >= 0; dedos_adriana--)
    v[(S + dedos_adriana)%N] = dedos_adriana;

  for (int dedos_necessarios : v) 
    std::cout << dedos_necessarios << ' ';
  std::cout << std::endl;

  return 0;
}