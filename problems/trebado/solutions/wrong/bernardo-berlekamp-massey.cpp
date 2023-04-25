#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

template<int p> struct mod_int {
	ll pow(ll b, ll e) {
		if (e == 0) return 1;
		ll r = pow(b*b%p, e/2);
		if (e%2 == 1) r = (r*b)%p;
		return r;
	}
	ll inv(ll b) { return pow(b, p-2); }

	using m = mod_int;
	int v;
	mod_int() : v(0) {}
	mod_int(ll v_) {
		if (v_ >= p or v_ <= -p) v_ %= p;
		if (v_ < 0) v_ += p;
		v = v_;
	}
	m& operator+=(const m &a) {
		v += a.v;
		if (v >= p) v -= p;
		return *this;
	}
	m& operator-=(const m &a) {
		v -= a.v;
		if (v < 0) v += p;
		return *this;
	}
	m& operator*=(const m &a) {
		v = v * ll(a.v) % p;
		return *this;
	}
	m& operator/=(const m &a) {
		v = v* inv(a.v) % p;
		return *this;
	}
	m operator-(){ return m(-v); }
	m& operator^=(ll e) {
		if (e < 0){
			v = inv(v);
			e = -e;
		}
		v = pow(v, e%(p-1));
		return *this;
	}
	bool operator==(const m &a) { return v == a.v; }
	bool operator!=(const m &a) { return v != a.v; }

	friend istream &operator>>(istream &in, m& a) {
		ll val; in >> val;
		a = m(val);
		return in;
	}
	friend ostream &operator<<(ostream &out, m a) {
		return out << a.v;
	}
	friend m operator+(m a, m b) { return a+=b; }
	friend m operator-(m a, m b) { return a-=b; }
	friend m operator*(m a, m b) { return a*=b; }
	friend m operator/(m a, m b) { return a/=b; }
	friend m operator^(m a, ll e) { return a^=e; }
};

typedef mod_int<(int)1e9+7> mint;

template<typename T> T evaluate(vector<T> c, vector<T> s, ll k) {
	int n = c.size();
	assert(c.size() <= s.size());

	auto mul = [&](const vector<T> &a, const vector<T> &b) {
		vector<T> ret(a.size() + b.size() - 1);
		for (int i = 0; i < a.size(); i++) for (int j = 0; j < b.size(); j++)
			ret[i+j] += a[i] * b[j];
		for (int i = ret.size()-1; i >= n; i--) for (int j = n-1; j >= 0; j--)
			ret[i-j-1] += ret[i] * c[j];
		ret.resize(min<int>(ret.size(), n));
		return ret;
	};

	vector<T> a = n == 1 ? vector<T>({c[0]}) : vector<T>({0, 1}), x = {1};
	while (k) {
		if (k&1) x = mul(x, a);
		a = mul(a, a), k >>= 1;
	}
	x.resize(n);

	T ret = 0;
	for (int i = 0; i < n; i++) ret += x[i] * s[i];
	return ret;
}

template<typename T> vector<T> berlekamp_massey(vector<T> s) {
    int n = s.size(), l = 0, m = 1;
    vector<T> b(n), c(n);
    T ld = b[0] = c[0] = 1;
    for (int i = 0; i < n; i++, m++) {
        T d = s[i];
        for (int j = 1; j <= l; j++) d += c[j] * s[i-j];
        if (d == 0) continue;
        vector<T> temp = c;
        T coef = d / ld;
        for (int j = m; j < n; j++) c[j] -= coef * b[j-m];
        if (2 * l <= i) l = i + 1 - l, b = temp, ld = d, m = 0;
    }
    c.resize(l + 1);
    c.erase(c.begin());
    for (T& x : c) x = -x;
    return c;
}

template<typename T> T guess_kth(const vector<T>& s, ll k) {
	auto c = berlekamp_massey(s);
	return evaluate(c, s, k);
}

const int MOD = 1e9+7, MAX = 5e3 + 10;

vector<mint> steal = {1, 
0, 
8, 
24, 
216, 
1200, 
8840, 
58800, 
423640, 
3000480, 
21824208, 
158964960, 
171230977, 
668531816, 
574843600, 
114852843, 
440874390, 
154500379, 
488604612, 
633055826, 
163941687};

int main() { _
	int k; cin >> k;
	cout << guess_kth(steal, k) << endl;

	exit(0);
}
