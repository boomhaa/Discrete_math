#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

void dfs(const vector<vector<int>>& graph, vector<int>& colors, int color, int node, int& edges) {
    colors[node] = color;
    for (int neigh : graph[node]) {
        edges++; // увеличиваем счетчик ребер
        if (colors[neigh] == -1) {
            dfs(graph, colors, color, neigh, edges);
        }
    }
}

int main() {
    int n, m, v, u;
    cin >> n >> m;
    vector<pair<int, int>> edges(m);
    vector<vector<int>> graph(n);
    for (int i = 0; i < m; i++) {
        cin >> v >> u;
        graph[v].push_back(u);
        graph[u].push_back(v);
        edges[i] = {v, u};
    }

    vector<int> colors(n, -1);
    vector<int> componentSize;
    vector<int> componentEdges;

    int curColor = 0;
    for (int i = 0; i < n; i++) {
        if (colors[i] == -1) {
            int edgeCount = 0;
            dfs(graph, colors, curColor, i, edgeCount);
            componentSize.push_back(count(colors.begin(), colors.end(), curColor));
            componentEdges.push_back(edgeCount / 2);
            curColor++;
        }
    }

    int color_win = -1;
    int max_vertices = -1;
    int max_edges = -1;

    for (int i = 0; i < curColor; i++) {
        if (componentSize[i] > max_vertices || (componentSize[i] == max_vertices && componentEdges[i] > max_edges)) {
            color_win = i;
            max_vertices = componentSize[i];
            max_edges = componentEdges[i];
        }
    }

    cout << "graph {" << endl;
    for (int i = 0; i < n; i++) {
        if (colors[i] == color_win) {
            cout << "    " << i << " [color = red]" << endl;
        } else {
            cout << "    " << i << endl;
        }
    }
    for (const auto& e : edges) {
        if (colors[e.first] == color_win && colors[e.second] == color_win) {
            cout << "    " << e.first << " -- " << e.second << " [color = red]" << endl;
        } else {
            cout << "    " << e.first << " -- " << e.second << endl;
        }
    }
    cout << "}" << endl;

    return 0;
}
