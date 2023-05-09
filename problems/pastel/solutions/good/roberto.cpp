#include <bits/stdc++.h>
#define int long long
using namespace std;
 
#define mp make_pair
#define mt make_tuple
#define pb push_back
#define ms(v, x) memset((v), (x), sizeof(v))
#define all(v) (v).begin(), (v).end()
#define ff first
#define ss second
#define iopt ios::sync_with_stdio(false); cin.tie(0)
 
int gcd(int a, int b) { return b == 0 ? a : gcd(b, a%b); }
int power(int x, int p, int MOD) {
    if(p == 0) return 1%MOD;
    if(p == 1) return x%MOD;
    int res = power(x, p/2, MOD);
    res = (long long)res*res%MOD;
    if(p&1) res = (long long)res*x%MOD;
    return res;
}
 
typedef pair<int, int> ii;
typedef long double LD;
typedef vector<int> vi;

const int N = 1e5+10;
const int B = 400;
const int M = (1 << 18);

struct Mex {
  vector<int> cnt, buckets;
  vector<int> added;
  Mex() {
    cnt = vector<int>(N + B);
    buckets = vector<int>((N + B) / B);
  }

  void add_from_row(int x) {
    added.pb(x);
    add_from_col(x);
  }

  void add_from_col(int x) {
    if (++cnt[x] == 1)
      buckets[x / B]++;
  }

  void erase_from_col(int x) {
    if (--cnt[x] == 0)
      buckets[x / B]--;
  }

  void clear_from_row() {
    for(int x : added)
      erase_from_col(x);
    added.clear();
  }

  int mex() const {
    for(int i = 0; i < buckets.size(); i++)
      if (buckets[i] < B) {
        for(int j = i * B; j < (i+1) * B; j++)
          if (!cnt[j]) return j;
      }
    assert(false);
  }
};

vector<vector<char>> transpose(const vector<vector<char>>& g) {
  int n = g.size(), m = g[0].size();
  
  vector<vector<char>> res(m, vector<char>(n, 'x'));
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < m; j++) {
      res[j][i] = g[i][j];
    }
  }
  return res;
}

vector<int> solve() {
  int n, m; cin >> n >> m;

  vector<vector<char>> g(n+1, vector<char>(m+1, 'x'));
  
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < m; j++) {
      cin >> g[i][j];
    }
  }

  if (n > m) g = transpose(g);
  n = g.size(), m = g[0].size();

  vector<vector<int>> res(n, vector<int>(m));

  for(int i = n-1; i >= 0; i--) {
    Mex mex;
    for(int j = m-1; j >= 0; j--) {
      if (g[i][j] == 'x') {
        mex.clear_from_row();
        continue;
      }
      for(int k = i+1; k < n; k++) {
        if (g[k][j] == 'x') break;
        mex.add_from_col(res[k][j]);
      }
      res[i][j] = mex.mex();
      for(int k = i+1; k < n; k++) {
        if (g[k][j] == 'x') break;
        mex.erase_from_col(res[k][j]);
      }
      mex.add_from_row(res[i][j]);
    }
  }

  vector<int> cands;
  for(int i = 0; i < n; i++) {
    if (g[i][0] == 'x') break;
    cands.pb(res[i][0]);
  }
  for(int i = 1; i < m; i++) {
    if (g[0][i] == 'x') break;
    cands.pb(res[0][i]);
  }

  return cands;
}

vector<int> build_hist(const vector<int>& v) {
  vector<int> res(M);
  for(int x : v) res[x]++;
  return res;
}

void fht(int * p, int n, bool inverse = false){
  for(int L = 2; L <= n; L <<= 1){
    for(int i = 0; i < n; i += L){
      for(int j = 0; j < L/2; j++){
        int u = p[i+j];
        int v = p[i+j+L/2];
        p[i+j] = u+v;
        p[i+j+L/2] = u-v;
      }
    }
  }

  if(inverse){
    for(int i = 0; i < n; i++){
      p[i] /= n;
    }
  }
}

int32_t main(){
    iopt;

    auto a = solve();
    auto b = solve();
    auto c = solve();

    a = build_hist(a), b = build_hist(b), c = build_hist(c);

    fht(a.data(), a.size());
    fht(b.data(), b.size());
    fht(c.data(), c.size());
    for(int i = 0; i < a.size(); i++) a[i] *= b[i]*c[i];
    fht(a.data(), a.size(), true);

    cout << a[0] << endl;
}
