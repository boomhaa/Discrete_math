#include <iostream>
#include <vector>

using namespace std;

int main() {
    const int inf = 1000000;
    int n,m,u,v,w,sum2=0;
    cin >> n >> m;
    vector<vector<int>> graph(n,vector<int> (n, inf));
    vector<bool> used(n);
    vector<int> min_e (n, inf), sel_e (n, -1);
    for (int i=0;i<m;i++) {
        cin >> u >> v >> w;
        graph[u][v] = min(graph[u][v],w);
        graph[v][u] = min(graph[v][u],w);
    }
    for (int i=0; i<n; ++i) {
        int v = -1;
        for (int j=0; j<n; ++j)
            if (!used[j] && (v == -1 || min_e[j] < min_e[v]))
                v = j;
        used[v] = true;
        if (sel_e[v] != -1) {
            sum2+=graph[v][sel_e[v]];
        }

        for (int to=0; to<n; ++to)
            if (graph[v][to] < min_e[to]) {
                min_e[to] = graph[v][to];
                sel_e[to] = v;
            }
    }
    cout <<sum2<<endl;
    return 0;
}