#include <iostream>
#include <vector>
#include <tuple>

using namespace std;
#define rep(i,a,b) for(int i=(a);i<(b);++i)
#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define trav(a, x) for(auto& a : x)
#define allin(a , x) for(auto a : x)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()


typedef long long ll;
typedef pair<ll,ll> pll;
typedef vector<ll> vl;
typedef vector<int> vi;
const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

vector<vector<int>> le() {
	int n, m; cin >> n >> m;
	vector<vector<int>> ret(n, vector<int>(m,0));
	for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) {
		char c; cin >> c;
		if (c == 'x') ret[i][j] = -1;
	}
	return ret;
}

vector<vector<int>> trans(vector<vector<int>> v) {
	int n = v.size(), m = v[0].size();
	vector<vector<int>> ret(m, vector<int>(n, int()));
	for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) ret[j][i] = v[i][j];
	return ret;
}

const int N = 1<<17;
int visi[N];

int prv[N];
int nxt[N];

void clear(int p){
    nxt[p] = prv[p] = p;
    visi[p]=0;
}
void activate(int p){
    if(visi[p])return;
    visi[p]=1;
    int ant = (p==0 || !visi[p-1] ? p : prv[p-1]);
    int Nxt = (!visi[p+1] ? p : nxt[p+1]);
    prv[Nxt] = ant;
    nxt[ant] = Nxt;
}
int act[N];
vector<vector<int>> invert(vector<vi> v){
    vector<vi> res = v;
    int n = v.size(),m=v[0].size();
    rep(i,0,n)rep(j,0,m)res[n-1-i][m-1-j] = v[i][j];
    return res;
}
vector<vector<int>> get_grundy(vector<vector<int>> v) {
	v = invert(v);
    int n = v.size();
    int m = v[0].size();
    int tot = n+m;
    vector<vector<int>> res(n,vi(m,-1));
    for(int i=0;i<n;i++){

        rep(k,0,tot+3){
            clear(k);
            visi[k]=0;
        }
        int last = -1;
        for(int j=0;j<m;j++){
            if(v[i][j] == -1){
                for(int k=last+1;k<j;k++){
                    clear(res[i][k]);
                    visi[res[i][k]]=0;
                }
                last = j;
                res[i][j]=-1;
                continue;
            }

            // achar meu mex:
            for(int ii=i-1;ii>=0 && v[ii][j]!=-1;ii--)
                act[res[ii][j]]=1;
            int mex=0;
            while(1){
                if(visi[mex])mex = nxt[mex]+1;
                else if(act[mex])mex++;
                else break;
            }
            for(int ii=i-1;ii>=0 && v[ii][j]!=-1;ii--)
                act[res[ii][j]]=0;

            res[i][j] = mex;
            // atualizar mex:
            activate(res[i][j]);
        }

    }

    return invert(res);
}

vector<vector<int>> grundy(vector<vector<int>> v) {
	//if (v.size() > v[0].size()) return trans(get_grundy(trans(v)));
	return get_grundy(v);
}
vector<ll> mov(vector<vector<int>> v) {
	int n = v.size(), m = v[0].size();
    v = grundy(v);
    int mx=0;
    rep(i,0,n)rep(j,0,m)mx=max(mx,v[i][j]);
	vector<ll> ret(N);
	for (int i = 0; i < n; i++) {
		if (v[i][0] >= 0) ret[v[i][0]]++;
		else break;
	}
	for (int j = 1; j < m; j++) {
		if (v[0][j] >= 0) ret[v[0][j]]++;
		else break;
	}
    return ret;
}

void FST(vl& a, bool inv) {
	for (int n = sz(a), step = 1; step < n; step *= 2) {
		for (int i = 0; i < n; i += 2 * step) rep(j,i,i+step) {
			ll &u = a[j], &v = a[j + step]; tie(u, v) =
				// inv ? pll(v - u, u) : pll(v, u + v); // AND
				// inv ? pll(v, u - v) : pll(u + v, u); // OR /// include-line
				make_pair(u + v, u - v);                   // XOR /// include-line
		}
	}
	if (inv) trav(x, a) x /= sz(a); // XOR only /// include-line
}
vl conv(vl a, vl b) {
	FST(a, 0); FST(b, 0);
	rep(i,0,sz(a)) a[i] *= b[i];
	FST(a, 1); return a;
}

int main() { _
    vl a = mov(le()),b = mov(le()), c = mov(le());
	cout << conv(a, conv(b,c))[0] << endl;
	exit(0);
}
