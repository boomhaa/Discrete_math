#include <iostream>
#include <vector>
#include <string>
using namespace std;

void printDot(vector<vector<int>>&delta, vector<vector<string>> f, int n, int m) {
    cout << "digraph {" << endl;
    cout << "    rankdir = LR;" << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cout << "    " << i << " -> " << delta[i][j]
                    << " [label=\"" << char('a' + j) << "(" << f[i][j] << ")\"];" << endl;
        }
    }

    cout << "}" << endl;

}

int main() {
    int n, m, q0;
    cin >> n >> m >> q0;
    vector<vector<int> > delta(n, vector<int>(m));
    vector<vector<string> > f(n, vector<string>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> delta[i][j];
        }
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> f[i][j];
        }
    }

    printDot(delta,f,n,m);

    return 0;
}
