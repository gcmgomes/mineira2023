#include <iostream>
#include <stack>
#include <vector>

using namespace std;

struct Node {
  Node(int i, int a, int b) : id(i), l(a), r(b) {}
  int id;
  int l;
  int r;
  bool Cross(const Node& n) {
    return (l < n.l && n.l < r && r < n.r) || (n.l < l && l < n.r && n.r < l);
  }
  vector<int> viz;
};

int DFS(const vector<Node>& graph) {
  vector<int> color(graph.size(), -1);
  vector<int> prev(graph.size(), -1);
  for (int i = 0; i < graph.size(); i++) {
    if (color[i] != -1) {
      continue;
    }
    stack<int> s;
    s.push(0);
    while (s.size()) {
      int v = s.top();
      color[v] = (prev[v] == -1) ? 0 : 1 - color[prev[v]];
      s.pop();
      for (auto& u : graph[v].viz) {
        if (color[u] == color[v]) {
          return 0;
        }
        if (color[u] == -1) {
          prev[u] = v;
          s.push(u);
        } else if (u != prev[v]) {
          cerr << "found a cycle" << endl;
        }
      }
    }
  }
  return 1;
}

int main() {
  int n = 0;
  cin >> n;
  vector<int> cristais(n);
  vector<vector<int>> cores(n);
  int z = 0;
  for (auto& c : cristais) {
    cin >> c;
    cores[c - 1].push_back(z++);
    if (cores[c - 1].size() > 4) {
      cout << 0 << endl;
      return 0;
    }
  }
  z = 0;
  vector<Node> graph;
  for (auto& c : cores) {
    for (int i = 0; i < c.size(); i++) {
      for (int j = i + 1; j < c.size(); j++) {
        graph.push_back(Node(z++, c[i], c[j]));
      }
    }
  }
  for (int i = 0; i < graph.size(); i++) {
    for (int j = 0; j < graph.size(); j++) {
      if (graph[i].Cross(graph[j])) {
        graph[i].viz.push_back(j);
        graph[j].viz.push_back(i);
      }
    }
  }
  cout << DFS(graph) << endl;
}
