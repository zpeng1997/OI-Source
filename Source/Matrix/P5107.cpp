#include <cstdio>
#include <limits>
#include <vector>
int read() {
    int res = 0, c;
    do
        c = getchar();
    while(c < '0' || c > '9');
    while('0' <= c && c <= '9') {
        res = res * 10 + c - '0';
        c = getchar();
    }
    return res;
}
typedef long long Int64;
const int mod = 998244353, size = 50;
const Int64 end = std::numeric_limits<Int64>::max() -
    (mod - 1LL) * (mod - 1LL);
struct Mat {
    Int64 A[size][size];
    int n, m;
    Mat() {}
    Mat(int n, int m) : n(n), m(m) {
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < m; ++j)
                A[i][j] = 0;
    }
    Mat operator*(const Mat& rhs) const {
        Mat res(n, rhs.m);
        for(int i = 0; i < n; ++i)
            for(int k = 0; k < m; ++k) {
                Int64 fac = A[i][k];
                for(int j = 0; j < rhs.m; ++j) {
                    Int64& val = res.A[i][j];
                    val += fac * rhs.A[k][j];
                    if(val > end)
                        val %= mod;
                }
            }
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < m; ++j)
                res.A[i][j] %= mod;
        return res;
    }
} B[32];
std::vector<int> G[55];
Int64 powm(Int64 a, int k) {
    Int64 res = 1;
    while(k) {
        if(k & 1)
            res = res * a % mod;
        k >>= 1, a = a * a % mod;
    }
    return res;
}
int main() {
    int n = read();
    int m = read();
    int q = read();
    Mat vec(1, n);
    for(int i = 0; i < n; ++i)
        vec.A[0][i] = read();
    for(int i = 0; i < m; ++i) {
        int u = read() - 1;
        int v = read() - 1;
        G[u].push_back(v);
    }
    Mat base(n, n);
    for(int i = 0; i < n; ++i) {
        G[i].push_back(i);
        Int64 div = powm(G[i].size(), mod - 2);
        for(int j = 0; j < G[i].size(); ++j) {
            Int64& val = base.A[i][G[i][j]];
            val = (val + div) % mod;
        }
    }
    B[0] = base;
    for(int i = 1; i < 30; ++i)
        B[i] = B[i - 1] * B[i - 1];
    for(int t = 0; t < q; ++t) {
        Mat res = vec;
        int k = read();
        for(int i = 0; i < 30; ++i, k >>= 1)
            if(k & 1)
                res = res * B[i];
        Int64 ans = 0;
        for(int i = 0; i < n; ++i)
            ans ^= res.A[0][i];
        printf("%lld\n", ans % mod);
    }
    return 0;
}
