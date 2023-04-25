#include <iostream>
#include <iomanip>

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

int main() { _
	int r; cin >> r;
	float th = atan(3.0 / 4.0);
	float ans = r * r * th + r * r * sin(2*th) / 2.0;
	cout << fixed << setprecision(2) << ans << endl;
	return 0;
}
