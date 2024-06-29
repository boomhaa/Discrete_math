#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct State {
    bool state;
    int or_name;
    int can_name;
    vector<int> *delta;
    vector<string> *f;
};

int cnt = 0;
vector<State *> res;
State *dat[100000];

void dfs(int cur_state, int n, int m) {
    State *cur = dat[cur_state];
    cur->can_name = cnt++;
    cur->state = true;
    res.push_back(cur);
    vector<pair<int, int> > massive_pairs;
    for (int i = 0; i < m; i++) {
        massive_pairs.push_back({i, dat[cur_state]->delta->at(i)});
    }
    sort(massive_pairs.begin(), massive_pairs.end());
    for (int i = 0; i < m; i++) {
        int next_state = massive_pairs[i].second;
        if (!dat[next_state]->state) {
            dfs(next_state, n, m);
        }
        cur->delta->at(i) = dat[next_state]->can_name;
    }
}

int main() {
    int n, m, q0, c;
    string s;
    cin >> n >> m >> q0;
    for (int i = 0; i < n; i++) {
        State *d = new State();
        d->delta = new vector<int>;
        d->state = false;
        d->or_name = i;
        d->can_name = -1;
        for (int j = 0; j < m; j++) {
            cin >> c;
            d->delta->push_back(c);
        }
        dat[i] = d;
    }
    for (int i = 0; i < n; i++) {
        dat[i]->f = new vector<string>;
        for (int j = 0; j < m; j++) {
            cin >> s;
            dat[i]->f->push_back(s);
        }
    }
    dfs(q0, n, m);
    cout << n << "\n" << m << "\n" << 0 << "\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << res[i]->delta->at(j) << " ";
        }
        cout << "\n";
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << res[i]->f->at(j) << " ";
        }
        cout << "\n";
    }
    return 0;
}
