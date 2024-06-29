#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <functional>

using namespace std;

struct DSU {
    vector<int> parent;
    vector<int> rank;

    void initDSU(int numQ) {
        parent.resize(numQ);
        rank.resize(numQ);
        for (int i = 0; i < numQ; i++) {
            parent[i] = i;
        }
    }

    int find(int q) {
        if (parent[q] != q) {
            parent[q] = find(parent[q]);
        }
        return parent[q];
    }

    void unionSets(int q1, int q2) {
        int p1 = find(q1);
        int p2 = find(q2);
        if (rank[p1] > rank[p2]) {
            swap(p1, p2);
        }
        parent[p1] = p2;
        if (rank[p1] == rank[p2]) {
            rank[p2]++;
        }
    }
};

pair<int, vector<int> > split1(int numQ, int numIn, const vector<vector<int> > &QInToQ,
                               const vector<vector<string> > &QInToOut) {
    int m = numQ;
    DSU dsuQ;
    dsuQ.initDSU(numQ);
    for (int q1 = 0; q1 < numQ; q1++) {
        for (int q2 = q1 + 1; q2 < numQ; q2++) {
            if (dsuQ.find(q1) == dsuQ.find(q2)) continue;
            bool eq = true;
            for (int x = 0; x < numIn; x++) {
                if (QInToOut[q1][x] != QInToOut[q2][x]) {
                    eq = false;
                    break;
                }
            }
            if (eq) {
                dsuQ.unionSets(q1, q2);
                m--;
            }
        }
    }
    vector<int> pi(numQ);
    for (int q = 0; q < numQ; q++) {
        pi[q] = dsuQ.find(q);
    }
    return {m, pi};
}

pair<int, vector<int> > split(int numQ, int numIn, const vector<vector<int> > &QInToQ,
                              const vector<vector<string> > &QInToOut, vector<int> &pi) {
    int m = numQ;
    DSU dsuQ;
    dsuQ.initDSU(numQ);
    for (int q1 = 0; q1 < numQ; q1++) {
        for (int q2 = q1 + 1; q2 < numQ; q2++) {
            if (pi[q1] != pi[q2] || dsuQ.find(q1) == dsuQ.find(q2)) continue;
            bool eq = true;
            for (int x = 0; x < numIn; x++) {
                if (pi[QInToQ[q1][x]] != pi[QInToQ[q2][x]]) {
                    eq = false;
                    break;
                }
            }
            if (eq) {
                dsuQ.unionSets(q1, q2);
                m--;
            }
        }
    }
    for (int q = 0; q < numQ; q++) {
        pi[q] = dsuQ.find(q);
    }
    return {m, pi};
}

tuple<int, int, vector<vector<int> >, vector<vector<string> > > aufenkampHohn(int numQ,
    int numIn, int startQ,const vector<vector<int> > &QInToQ,
    const vector<vector<string> > &QInToOut) {
    auto [m, pi] = split1(numQ, numIn, QInToQ, QInToOut);
    while (true) {
        auto [newM, newPi] = split(numQ, numIn, QInToQ, QInToOut, pi);
        if (m == newM) break;
        m = newM;
        pi = newPi;
    }

    map<int, int> classIndQ, classIndToQ, QToClassInd;
    int classCnt = 0;
    for (int q = 0; q < numQ; q++) {
        int classQ = pi[q];
        if (classIndQ.find(classQ) != classIndQ.end()) continue;
        classIndToQ[classCnt] = classQ;
        classIndQ[classQ] = q;
        QToClassInd[classQ] = classCnt++;
    }

    vector<vector<int> > newQInToQ(classCnt, vector<int>(numIn));
    vector<vector<string> > newQInToOut(classCnt, vector<string>(numIn));
    for (int classInd = 0; classInd < classCnt; classInd++) {
        int q = classIndQ[classIndToQ[classInd]];
        for (int x = 0; x < numIn; x++) {
            newQInToQ[classInd][x] = QToClassInd[pi[QInToQ[q][x]]];
            newQInToOut[classInd][x] = QInToOut[q][x];
        }
    }

    int newStartQ = QToClassInd[pi[startQ]];
    int newNumQ = m;
    return {newNumQ, newStartQ, newQInToQ, newQInToOut};
}

pair<map<int, int>, map<int, int> > numerate(int numQ, int numIn, int startQ,
                                             const vector<vector<int> > &QInToQ) {
    int time = 0;
    map<int, int> TimeToQ, newQ;
    set<int> used;
    function<void(int)> dfs = [&](int q) {
        used.insert(q);
        TimeToQ[time] = q;
        newQ[q] = time++;
        for (int to: QInToQ[q]) {
            if (used.find(to) == used.end()) {
                dfs(to);
            }
        }
    };

    dfs(startQ);
    for (int i = 0; i < numQ; i++) {
        if (used.find(i) == used.end()) {
            dfs(i);
        }
    }

    return {TimeToQ, newQ};
}


void scanAutomat(int &n, int &m, int &q0, vector<vector<int> > &delta, vector<vector<string> > &f) {
    cin >> n >> m >> q0;
    delta.resize(n, vector<int>(m));
    f.resize(n, vector<string>(m));

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
}

bool compare(int new_n1, int m1, vector<vector<int> > new_delta1, vector<vector<string> > new_f1,
             map<int, int> TimeToQ1, map<int, int> newQ1,
             int new_n2, int m2, vector<vector<int> > new_delta2, vector<vector<string> > new_f2,
             map<int, int> TimeToQ2, map<int, int> newQ2) {
    if (new_n1 != new_n2 || m1 != m2) {
        return false;
    }
    for (int i = 0; i < new_n1; i++) {
        for (int j = 0; j < m1; j++) {
            if (newQ1[new_delta1[TimeToQ1[i]][j]] != newQ2[new_delta2[TimeToQ2[i]][j]]
                || new_f1[TimeToQ1[i]][j] != new_f2[TimeToQ2[i]][j]) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    int n1, m1, q01, n2, m2, q02;
    vector<vector<int> > delta1, delta2;
    vector<vector<string> > f1, f2;


    scanAutomat(n1, m1, q01, delta1, f1);
    scanAutomat(n2, m2, q02, delta2, f2);

    auto [new_n1, new_q01, new_delta1, new_f1] = aufenkampHohn(n1, m1, q01, delta1, f1);
    auto [new_n2, new_q02, new_delta2, new_f2] = aufenkampHohn(n2, m2, q02, delta2, f2);

    auto [TimeToQ1, newQ1] = numerate(new_n1, m1, new_q01, new_delta1);
    auto [TimeToQ2, newQ2] = numerate(new_n2, m2, new_q02, new_delta2);
    if (compare(new_n1,m1, new_delta1,new_f1, TimeToQ1, newQ1, new_n2,m2,
        new_delta2,new_f2, TimeToQ2, newQ2)) {
        cout << "EQUAL";
    }
    else {
        cout << "NOT EQUAL";
    }
    return 0;
}
