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

const int MOD = 1e9+7;

int32_t main(){
    iopt;

    int n; cin >> n;

    set<int> primes;
    for(int i = 0; i < n; i++) {
      int a, b; cin >> a >> b;
      int x = b / gcd(a, b);
      for(int i = 2; i*i <= x; i++) {
        if (x % i == 0) {
          primes.insert(i);
        }
        while(x % i == 0) x /= i;
      }
      if (x > 1) primes.insert(x);
    }

    int ans = 1;
    for(int x : primes)
      ans = (ans * x) % MOD;
	if (primes.empty()) cout << 2 << endl;
    else cout << ans << endl;
}
