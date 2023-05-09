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

const int M = 20;
const vector<char> vowels = {'a', 'e', 'i', 'o', 'u', 'y'};

int get(char c) {
  return c-'a' - (upper_bound(all(vowels), c) - vowels.begin());
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
    int n; cin >> n;
    string s; cin >> s;

    vector<int> hist(1 << M);
    int acc = 0;
    hist[0]++;

    for(char c : s) {
      acc ^= (1ll << get(c));
      //cout << get(c);
      hist[acc]++;
    }
    //cout << endl;

    vector<int> a = hist, b = hist;
    fht(a.data(), a.size());
    fht(b.data(), b.size());
    for(int i = 0; i < a.size(); i++) a[i] *= b[i];
    fht(a.data(), a.size(), true);

    a[0] -= n+1;
    for(int i = 0; i < a.size(); i++) a[i] /= 2;

    int ans = a[0];
    for(int i = 0; i < M; i++)
      ans += a[1 << i];
    cout << ans << endl;
}
