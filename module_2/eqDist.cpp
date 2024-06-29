#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits.h>

using namespace std;

void bfs(const vector<vector<int>>& graph, int start, vector<int>& distances) {
    queue<int> q;
    q.push(start);
    distances[start] = 0;

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (int neighbor : graph[node]) {
            if (distances[neighbor] == INT_MAX) {
                distances[neighbor] = distances[node] + 1;
                q.push(neighbor);
            }
        }
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> graph(n);

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    int k;
    cin >> k;
    vector<int> specialVertices(k);

    for (int i = 0; i < k; i++) {
        cin >> specialVertices[i];
    }

    vector<vector<int>> distances(k, vector<int>(n, INT_MAX));

    for (int i = 0; i < k; i++) {
        bfs(graph, specialVertices[i], distances[i]);
    }

    vector<int> result;

    for (int i = 0; i < n; i++) {
        bool isEqual = true;
        int dist = distances[0][i];

        for (int j = 1; j < k; j++) {
            if (distances[j][i] != dist) {
                isEqual = false;
                break;
            }
        }

        if (isEqual && dist != INT_MAX) {
            result.push_back(i);
        }
    }

    if (result.empty()) {
        cout << "-" << endl;
    } else {
        sort(result.begin(), result.end());
        for (int v : result) {
            cout << v << " ";
        }
        cout << endl;
    }

    return 0;
}
