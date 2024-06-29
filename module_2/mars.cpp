#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

bool dfs(int node, int color, vector<vector<int> > &graph, vector<int> &colors) {
    colors[node] = color;
    for (int neigh: graph[node]) {
        if (colors[neigh] == -1) {
            if (!dfs(neigh, 1 - color, graph, colors)) {
                return false;
            }
        } else if (colors[neigh] == color) {
            return false;
        }
    }

    return true;
}

int main() {
    int n;
    char c;
    cin >> n;
    vector<vector<int> > graph(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> c;
            if (c == '+') {
                graph[i].push_back(j);
            }
        }
    }


    vector<int> colors(n, -1);
    bool isDobledols = true;
    for (int i = 0; i < n; i++) {
        if (colors[i] == -1 && graph[i].size() > 0) {
            if (!dfs(i, 0, graph, colors)) {
                isDobledols = false;
                break;
            }
        }
    }
    if (!isDobledols) {
        cout << "No solution" << endl;
    } else {
        vector<int> group1, group2;
        for (int i = 0; i < n; ++i) {
            if (colors[i] == 0) {
                group1.push_back(i + 1);
            } else if (colors[i] == 1) {
                group2.push_back(i + 1);
            }
        }

        int cnt = 0;
        int len = n / 2 - group1.size();
        for (int i = 0; i < n; i++) {
            if (colors[i] == -1) {
                cnt++;
                if (cnt <= len) {
                    group1.push_back(i + 1);
                } else {
                    group2.push_back(i + 1);
                }
            }
        }
        sort(group1.begin(), group1.end());
        sort(group2.begin(), group2.end(1));
        if (group1.size() > group2.size() || (group1.size() == group2.size() && group1 > group2)) {
            swap(group1, group2);
        }

        for (int i = 0; i < group1.size(); ++i) {
            if (i > 0) cout << " ";
            cout << group1[i];
        }
        cout << endl;
    }
    return 0;
}
