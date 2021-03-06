#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>
const int size = 2010, inf = 0x3f3f3f3f;
struct Edge {
    int to, rev, f, w;
    Edge(int to, int rev, int f, int w)
        : to(to), rev(rev), f(f), w(w) {}
};
std::vector<Edge> G[size];
void addEdge(int u, int v, int f, int w) {
    int usiz = G[u].size(), vsiz = G[v].size();
    G[u].push_back(Edge(v, vsiz, f, w));
    G[v].push_back(Edge(u, usiz, 0, -w));
}
int dis[size], q[size], S, T;
bool flag[size];
bool SPFA(int n) {
    memset(dis + 1, 0xc0, sizeof(int) * n);
    dis[T] = 0, flag[T] = true, q[0] = T;
    int b = 0, e = 1;
    while(b != e) {
        int u = q[b++];
        if(b == size)
            b = 0;
        flag[u] = false;
        for(auto E : G[u]) {
            int v = E.to;
            int dv = dis[u] - E.w;
            if(G[v][E.rev].f && dv > dis[v]) {
                dis[v] = dv;
                if(!flag[v]) {
                    flag[v] = true;
                    if(b == e ||
                       dis[q[b]] + 30 >= dis[v]) {
                        q[e++] = v;
                        if(e == size)
                            e = 0;
                    } else {
                        if(--b == -1)
                            b = size - 1;
                        q[b] = v;
                    }
                }
            }
        }
    }
    return dis[S] != 0xc0c0c0c0;
}
int now[size];
int DFS(int u, int f) {
    if(u == T || f == 0)
        return f;
    flag[u] = true;
    int res = 0, k;
    for(int& i = now[u]; i < G[u].size(); ++i) {
        Edge& E = G[u][i];
        int v = E.to;
        if(E.f && !flag[v] && dis[u] == dis[v] + E.w &&
           (k = DFS(v, std::min(f, E.f)))) {
            E.f -= k, G[v][E.rev].f += k;
            res += k, f -= k;
            if(f == 0)
                break;
        }
    }
    flag[u] = false;
    if(!res)
        dis[u] = 0xc0c0c0c0;
    return res;
}
int MCMF(int n) {
    int res = 0;
    while(SPFA(n)) {
        memset(now + 1, 0, sizeof(int) * n);
        int cf = 0, k, cd = dis[S];
        while((k = DFS(S, inf)))
            cf += k;
        res += cf * cd;
    }
    return res;
}
int L[size], R[size], P[size];
int find(int x, int siz) {
    return std::lower_bound(P + 1, P + siz + 1, x) - P;
}
int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    int pcnt = 0;
    for(int i = 1; i <= n; ++i) {
        scanf("%d%d", &L[i], &R[i]);
        if(L[i] > R[i])
            std::swap(L[i], R[i]);
        P[++pcnt] = L[i];
        P[++pcnt] = R[i];
    }
    std::sort(P + 1, P + pcnt + 1);
    pcnt = std::unique(P + 1, P + pcnt + 1) - (P + 1);
    for(int i = 1; i < pcnt; ++i)
        addEdge(i, i + 1, inf, 0);
    T = pcnt;
    S = T + 1;
    for(int i = 1; i <= n; ++i) {
        int w = R[i] - L[i];
        int lp = find(L[i], pcnt);
        int rp = find(R[i], pcnt);
        addEdge(lp, rp, 1, w);
    }
    addEdge(S, 1, k, 0);
    printf("%d\n", MCMF(pcnt + 1));
    return 0;
}
