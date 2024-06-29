#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

struct edge {
    int u, v;
    double w;

    bool operator<(const edge &other) {
        return w < other.w;
    }
};

int main() {
    int n, x, y;
    cin >> n;
    vector<pair<int, int> > coords(n);
    for (int i = 0; i < n; i++) {
        cin >> x >> y;
        coords[i].first = x;
        coords[i].second = y;
    }

    vector<edge> graph;

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (i != j) {
                edge e;
                e.w = sqrt(pow(coords[i].first - coords[j].first, 2) + pow(coords[i].second - coords[j].second, 2));
                e.u = i;
                e.v = j;
                graph.push_back(e);
            }
        }
    }

    sort(graph.begin(), graph.end());
    double cost = 0;
    vector<int> tree_id(n);
    for (int i = 0; i < n; i++) {
        tree_id[i] = i;
    }
    for (edge eg: graph) {
        int a = eg.u, b = eg.v;
        double weight = eg.w;
        if (tree_id[a] != tree_id[b]) {
            cost += weight;
            int old_id = tree_id[b], new_id = tree_id[a];
            for (int j = 0; j < n; j++) {
                if (tree_id[j] == old_id) {
                    tree_id[j] = new_id;
                }
            }
        }
    }
    cout <<fixed<<setprecision(2) << cost << endl;
    return 0;
}
