#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int> > > graph(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b, c;
        cin >> a >> b >> c;
        graph[a].push_back({b, c});
        graph[b].push_back({a, c});
    }

    priority_queue<tuple<int, vector<int>, int>,
        vector<tuple<int, vector<int>, int> >,
        greater<tuple<int, vector<int>, int> > > pq;

    pq.push({0, {}, 1});

    vector<bool> visited(n + 1, false);
    vector<vector<int> > best_path(n + 1);

    while (!pq.empty()) {
        auto [path_length, path, node] = pq.top();
        pq.pop();

        if (visited[node]) {
            continue;
        }
        visited[node] = true;
        best_path[node] = path;

        if (node == n) {
            cout << path.size() << endl;
            for (int color: path) {
                cout << color << " ";
            }
            cout << endl;
            return 0;
        }

        for (const pair<int, int> &edge: graph[node]) {
            if (!visited[edge.first]) {
                vector<int> new_path = path;
                new_path.push_back(edge.second);
                pq.push({new_path.size(), new_path, edge.first});
            }
        }
    }

    return 0;
}
