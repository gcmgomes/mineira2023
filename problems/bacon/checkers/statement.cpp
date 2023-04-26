#include "testlib.h"
#include <vector>

using namespace std;

int dfs(int v, std::vector<bool>& vis, const std::vector<std::vector<int>>& g) {
  vis[v] = true;
  int n_vis = 1;
  for (int u : g[v]) if (not vis[u]) n_vis += dfs(u, vis, g);
  return n_vis;
}

int main(int argc, char* argv[]) {
  registerValidation(argc, argv);

  int n = inf.readInt(1, 1e5);
  inf.readEoln();
  
  std::vector<std::vector<int>> g(n);
  for (int i = 0; i < n - 1; i++) {
    int a = inf.readInt(1, n);
    inf.readSpace();
    int b = inf.readInt(1, n);
    inf.readEoln();

    g[a - 1].push_back(b - 1);
    g[b - 1].push_back(a - 1);
  }

  std::vector<bool> vis(n);
  ensure(dfs(0, vis, g) == n);

  inf.readEof();
}
