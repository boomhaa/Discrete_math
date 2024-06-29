#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void dfs(int v, int parent, vector<vector<int> > &graph, vector<bool> &visited, vector<int> &tin, vector<int> &low,
         int &timer, vector<pair<int, int> > &bridges) {
    visited[v] = true;
    tin[v] = low[v] = timer++;
    for (int to: graph[v]) {
        if (to == parent) continue;
        if (visited[to]) {
            low[v] = min(low[v], tin[to]);
        } else {
            dfs(to, v, graph, visited, tin, low, timer, bridges);
            low[v] = min(low[v], low[to]);
            if (low[to] > tin[v]) {
                bridges.push_back({v, to});
            }
        }
    }
}

int findBridges(int n, vector<vector<int> > &graph) {
    vector<bool> visited(n, false);
    vector<int> tin(n, -1);
    vector<int> low(n, -1);
    vector<pair<int, int> > bridges;
    int timer = 0;

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            dfs(i, -1, graph, visited, tin, low, timer, bridges);
        }
    }

    return bridges.size();
}

int main() {
    int n, m;
    cin >> n;
    cin >> m;
    vector<vector<int> > graph(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    cout << findBridges(n, graph) << endl;

    return 0;
}
