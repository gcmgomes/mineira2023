#include <bits/stdc++.h>

using namespace std;

constexpr int MAX_N = 112345;

int n;
vector<int> g[MAX_N];
int in[MAX_N];
int out[MAX_N];
int nxt[MAX_N];
int parent[MAX_N];
int depth[MAX_N];
int sz[MAX_N];
int seq[MAX_N];
int t;

int st[4 * MAX_N];
int lazy[4 * MAX_N];

void fix(int nl, int nr, int id) {
  if (lazy[id] == -1)
    return;
  st[id] = (nr - nl) * lazy[id];
  if (nl + 1 < nr) {
    lazy[2 * id] = lazy[id];
    lazy[2 * id + 1] = lazy[id];
  }
  lazy[id] = -1;
}

int query(int l, int r, int nl = 0, int nr = n, int id = 1) {
  if (l >= r || r <= nl || l >= nr)
    return 0;
  fix(nl, nr, id);
  if (l <= nl && nr <= r)
    return st[id];
  int mid = (nl + nr) / 2;
  return query(l, r, nl, mid, 2 * id) + query(l, r, mid, nr, 2 * id + 1);
}

int update(int l, int r, int v, int nl = 0, int nr = n, int id = 1) {
  if (l >= r || nl >= nr)
    return 0;
  fix(nl, nr, id);
  if (r <= nl || l >= nr) {
    return st[id];
  }
  if (l <= nl && nr <= r) {
    lazy[id] = v;
    fix(nl, nr, id);
    return st[id];
  }
  int mid = (nl + nr) / 2;
  st[id] =
      update(l, r, v, nl, mid, 2 * id) + update(l, r, v, mid, nr, 2 * id + 1);
  return st[id];
}

void dfs_sz(int v = 0, int p = -1) {
  sz[v] = 1;
  for (auto &u : g[v]) {
    if (u == p)
      continue;
    dfs_sz(u, v);
    sz[v] += sz[u];
    if (sz[u] > sz[g[v][0]]) {
      swap(u, g[v][0]);
    }
  }
}

void dfs_hld(int v = 0, int p = -1) {
  in[v] = t++;
  seq[in[v]] = v;
  parent[v] = p;
  if (p != -1)
    depth[v] = depth[p] + 1;
  for (auto u : g[v]) {
    if (u == p)
      continue;
    nxt[u] = (u == g[v][0] ? nxt[v] : u);
    dfs_hld(u, v);
  }
  out[v] = t;
}

bool is_full(int x, int v) {
  return query(in[x], out[x]) >= sz[x] - depth[v] + depth[x];
}

void paint(int v) {
  if (query(in[v], out[v]) == sz[v])
    return;

  update(in[v], out[v], 1);

  int x = parent[v];
  while (x != -1 && is_full(nxt[x], v)) {
    update(in[nxt[x]], in[x] + 1, 1);
    v = nxt[x];
    x = parent[nxt[x]];
  }

  if (x == -1)
    return;

  int lb = in[nxt[x]];
  int ub = in[x] + 1;

  while (lb + 1 < ub) {
    int mid = (lb + ub - 1) / 2;
    if (is_full(seq[mid], v)) {
      ub = mid + 1;
    } else {
      lb = mid + 1;
    }
  }
  if (is_full(seq[lb], v))
    update(lb, in[x] + 1, 1);
}

void unpaint(int v) {
  update(in[v], out[v], 0);
  while (v != -1) {
    update(in[nxt[v]], in[v] + 1, 0);
    v = parent[nxt[v]];
  }
}

int count(int v) { return query(in[v], out[v]); }

int main() {
  ios::sync_with_stdio(false);
  
  cin >> n;
  for (int i = 0; i < n - 1; i++) {
    int a, b;
    cin >> a >> b;
    a--;
    b--;
    g[a].push_back(b);
    g[b].push_back(a);
  }

  dfs_sz(0);
  parent[0] = -1;
  dfs_hld(0);

  int q;
  cin >> q;
  for (int qi = 0; qi < q; qi++) {
    int t, v;
    cin >> t >> v;
    v--;
    if (t == 1) {
      paint(v);
    } else if (t == 2) {
      unpaint(v);
    } else {
      cout << count(v) << '\n';
    }
  }
}
