#include<bits/stdc++.h>
using namespace std;

#define ll       long long
#define ull      unsigned ll
#define pii      pair<int, int>
#define pll      pair<ll, ll>
#define mp       make_pair
#define ff       first
#define ss       second
#define sz(x)    (int) x.size()
#define all(v)   v.begin(), v.end()

template <typename T> inline void smin(T &a, T b) {a = a < b ? a : b;}
template <typename T> inline void smax(T &a, T b) {a = a > b ? a : b;}

#define error(args...) { string _s = #args; replace(_s.begin(), _s.end(), ',', ' '); stringstream _ss(_s); istream_iterator<string> _it(_ss); err(_it, args); }
void err(istream_iterator<string> it) {cout << endl;}
template<typename T, typename... Args> void err(istream_iterator<string> it, T a, Args... args) {
	cerr << *it << " = " << a << ", ";
	err(++it, args...);
}

template <typename T> inline void Int(T &n) {
	n = 0; int f = 1; register int ch = getchar();
	for (; !isdigit(ch); ch = getchar()) if (ch == '-') f = -1;
	for (; isdigit(ch); ch = getchar()) n = (n << 3) + (n << 1) + ch - '0';
	n = n * f;
}

template <typename T, typename TT> inline void Int(T &n, TT &m) { Int(n); Int(m); }
template <typename T, typename TT, typename TTT> inline void Int(T &n, TT &m, TTT &l) { Int(n, m); Int(l); }

const int mod = (int) 1e9 + 7;

inline int add(int a, int b) {a += b; return a >= mod ? a - mod : a;}
inline int sub(int a, int b) {a -= b; return a < 0 ? a + mod : a;}
inline int mul(int a, int b) {return (ll) a * b % mod;}

const int inf = (int) 2e9 + 5;
const ll  Inf = (ll) 2e18 + 5;
const int N   = (int) 3e4 + 5;
const int Log = (int) 17;

std::vector<int> g[N];
std::vector<int> chain[N];
std::vector<int> bit[N];
int pos[N], idx[N], par[Log][N], dep[N], sz[N];
int num = 1;
int a[N];

void update(int n, int c, int x, int val) {
	while (x <= n) {
		bit[c][x] += val;
		x += (x & -x);
	}
}

int query(int c, int x) {
	int ans = 0;
	while (x > 0) {
		ans += bit[c][x];
		x -= (x & -x);
	}
	return ans;
}

void dfs(int u, int p = -1, int d = 0) {
	sz[u] = 1, dep[u] = d;
	for (int i = 1; i < Log; i++) {
		par[i][u] = par[i - 1][par[i - 1][u]];
	}
	for (int v : g[u]) {
		if (p == v) continue;
		par[0][v] = u;
		dfs(v, u, d + 1);
		sz[u] += sz[v];
	}
}

int lca(int u, int v) {
	if (dep[u] < dep[v]) swap(u, v);
	for (int i = Log - 1; i >= 0; i--) {
		if (dep[par[i][u]] >= dep[v]) u = par[i][u];
	}
	if (u == v) return u;
	for (int i = Log - 1; i >= 0; i--) {
		if (par[i][u] != par[i][v]) u = par[i][u], v = par[i][v];
	}
	return par[0][u];
}

void hld(int u, int p = -1) {
	chain[num].push_back(u);
	idx[u] = num; pos[u] = sz(chain[num]);
	int mx = -1, big = -1;
	for (int v : g[u]) {
		if (p != v and sz[v] > mx) mx = sz[v], big = v;
	}
	if (big != -1) hld(big, u);
	for (int v : g[u]) {
		if (p != v and v != big) ++num, hld(v, u);
	}
}

void process() {
	for (int i = 1; i <= num; i++) {
		for (int j = 0; j <= sz(chain[i]); j++) {
			bit[i].push_back(0);
		}
		for (int j = 0; j < sz(chain[i]); j++) {
			update(sz(chain[i]), i, j + 1, a[chain[i][j]]);
		}
	}
}

void update(int u, int val) {
	int c = idx[u], x = pos[u];
	update(sz(chain[c]), c, x, val);
}

int hld_query(int u, int v) {
	int l = lca(u, v), ans = 0;
	while (1) {
		int c = idx[u];
		if (idx[u] == idx[l]) {
			ans += query(c, pos[u]) - query(c, pos[l] - 1);
			break;
		}
		ans += query(c, pos[u]);
		u = par[0][chain[c][0]];
	}
	while (1) {
		int c = idx[v];
		if (idx[v] == idx[l]) {
			ans += query(c, pos[v]) - query(c, pos[l]);
			break;
		}
		ans += query(c, pos[v]);
		v = par[0][chain[c][0]];
	}
	return ans;
}

int solve() {
	int n; Int(n);
	for (int i = 1; i <= n; i++) {
		Int(a[i]);
	}
	for (int i = 1; i < n; i++) {
		int u, v; Int(u, v); ++u, ++v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	par[0][1] = 1;
	num = 1;
	dfs(1);
	hld(1);
	process();
	int q; Int(q);
	while (q--) {
		int ck; Int(ck);
		if (ck == 1) {
			int u, val; Int(u, val); ++u;
			update(u, val - a[u]);
			a[u] = val;
		}
		else {
			int u, v; Int(u, v); ++u, ++v;
			printf("%d\n", hld_query(u, v));
		}
	}
	return 0;
}

int main() {
	//ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	int test = 1, tc = 0;
	//Int(test);
	//cin >> test;
	while (test--) {
		//printf("Case %d:\n", ++tc);
		solve();
	}
	return 0;
}
