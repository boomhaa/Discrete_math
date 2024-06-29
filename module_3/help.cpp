#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;

vector<int> parent;
vector<int> rank1;

void make_set(int v) {
    parent[v] = v;
    rank1[v] = 0;
}

int find_set(int v) {
    if (v == parent[v]) {
        return v;
    }
    parent[v] = find_set(parent[v]);
    return parent[v];
}

void union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (rank1[a] < rank1[b]) {
            swap(a, b);
        }
        parent[b] = a;
        if (rank1[a] == rank1[b]) {
            rank1[a]++;
        }
    }
}

pair<int, vector<int> > split1(vector<vector<int> > &delta, vector<vector<string> > &f) {
    int m = delta.size();
    for (int i = 0; i < delta.size(); i++) {
        parent[i] = i;
    }
    for (int i = 0; i < delta.size(); i++) {
        for (int j = 0; j < delta.size(); j++) {
            if (find_set(i) != find_set(j)) {
                bool eq = true;
                for (int k = 0; k < delta[0].size(); k++) {
                    if (f[i][k] != f[j][k]) {
                        eq = false;
                        break;
                    }
                }
                if (eq) {
                    union_sets(i, j);
                    m--;
                }
            }
        }
    }
    vector<int> arr(delta.size());
    for (int i = 0; i < delta.size(); i++) {
        arr[i] = find_set(i);
    }
    return {m, arr};
}

int split(vector<vector<int> > &delta, vector<vector<string> > &f, vector<int> &arr) {
    int m = delta.size();
    for (int i = 0; i < delta.size(); i++) {
        parent[i] = i;
    }
    for (int i = 0; i < delta.size(); i++) {
        for (int j = 0; j < delta.size(); j++) {
            if (arr[i] == arr[j] && find_set(i) != find_set(j)) {
                bool eq = true;
                for (int k = 0; k < delta[0].size(); k++) {
                    int w1 = delta[i][k];
                    int w2 = delta[j][k];
                    if (arr[w1] != arr[w2]) {
                        eq = false;
                        break;
                    }
                }
                if (eq) {
                    union_sets(i, j);
                    m--;
                }
            }
        }
    }
    for (int i = 0; i < delta.size(); i++) {
        arr[i] = find_set(i);
    }
    return m;
}

tuple<vector<vector<int> >, vector<vector<string> >, vector<int>,
    vector<int> > minimize(vector<vector<int> > &delta, vector<vector<string> > &f) {
    auto [m, arr] = split1(delta, f);
    while (true) {
        int m1 = split(delta, f, arr);
        if (m == m1) {
            break;
        }
        m = m1;
    }

    vector<vector<int> > new_delta(delta.size(), vector<int>(delta[0].size()));
    vector<vector<string> > new_f(f.size(), vector<string>(f[0].size()));
    map<int, bool> used;

    vector<int> backmapping(delta.size());
    int cur = 0;
    for (int i = 0; i < delta.size(); i++) {
        int i1 = arr[i];
        backmapping[i] = cur;
        if (!used[i1]) {
            used[i1] = true;
            cur++;
        }
    }

    cur = 0;
    used.clear();
    for (int i = 0; i < delta.size(); i++) {
        int i1 = arr[i];
        if (!used[i1]) {
            used[i1] = true;
            for (int j = 0; j < delta[0].size(); j++) {
                new_delta[cur][j] = backmapping[arr[delta[i][j]]];
                new_f[cur][j] = f[i][j];
            }
            cur++;
        }
    }
    return {new_delta, new_f, backmapping, arr};
}

void dfs(int v, int &count, vector<vector<int> > &delta, vector<bool> &visited,
         vector<int> &num, vector<int> &anti_num) {
    visited[v] = true;
    num[count] = v;
    anti_num[v] = count;
    count++;
    for (int i = 0; i < delta[v].size(); i++) {
        int to = delta[v][i];
        if (visited[to]) {
            continue;
        }
        dfs(to, count, delta, visited, num, anti_num);
    }
}

int main() {
    int n, m, q0;
    cin >> n >> m >> q0;
    vector<vector<int> > delta(n, vector<int>(m));
    vector<vector<string> > f(n, vector<string>(m));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> delta[i][j];
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> f[i][j];
        }
    }

    parent.resize(n);
    rank1.resize(n);

    auto [new_delta, new_f, back, massive] = minimize(delta, f);
    vector<int> num(n);
    vector<int> anti_num(n);
    vector<bool> visited(n, false);
    int count = 0;
    q0 = back[massive[q0]];
    dfs(q0, count, new_delta, visited, num, anti_num);
    q0 = 0;

    cout << "digraph {\n\trankdir = LR" << endl;
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < new_delta[i].size(); j++) {
            cout << "\t" << i << " -> " << anti_num[new_delta[num[i]][j]] << " [label = \""
                    << char('a' + j) << "(" << new_f[num[i]][j] << ")\"]" << endl;
        }
    }
    cout << "}" << endl;
    return 0;
}
