#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct State {
    string word;
    int curQ;

    bool operator==(const State &other) const {
        return curQ == other.curQ && word == other.word;
    }
};

struct Edge {
    int from;
    int to;

    bool operator==(const Edge &other) const {
        return from == other.from && to == other.to;
    }
};

namespace std {
    template <>
    struct hash<State> {
        size_t operator()(const State &s) const {
            return hash<string>()(s.word) ^ hash<int>()(s.curQ);
        }
    };

    template <>
    struct hash<Edge> {
        size_t operator()(const Edge &e) const {
            return hash<int>()(e.from) ^ hash<int>()(e.to);
        }
    };
}


void recursiveGenerate(int curLen, const string &word, int curQ, const vector<vector<int> > &QInToQ,
                       const vector<vector<string> > &QInToOut, int maxLen, unordered_set<string> &language,
                       unordered_map<State, bool> &memo, unordered_set<Edge> &lambdaQ) {
    memo[{word, curQ}] = true;
    if (curLen > maxLen) {
        throw runtime_error("curLen > maxLen");
    }
    if (curLen == maxLen) {
        language.insert(word);
        return;
    }
    if (!word.empty()) {
        language.insert(word);
    }
    int numIn = QInToQ[0].size();
    for (int i = 0; i < numIn; ++i) {
        int newQ = QInToQ[curQ][i];
        string newSymbol = QInToOut[curQ][i];
        Edge newEdge = {curQ, newQ};
        if (lambdaQ.find(newEdge) != lambdaQ.end() || (newQ == curQ && newSymbol == "-")) {
            continue;
        }
        if (memo.find({word + newSymbol, newQ}) != memo.end()) {
            continue;
        }
        if (newSymbol == "-") {
            lambdaQ.insert(newEdge);
            recursiveGenerate(curLen, word, newQ, QInToQ, QInToOut, maxLen, language, memo, lambdaQ);
            lambdaQ.erase(newEdge);
        } else {
            unordered_set<Edge> newLambdaQ;
            recursiveGenerate(curLen + 1, word + newSymbol, newQ, QInToQ, QInToOut,
                              maxLen, language, memo, newLambdaQ);
        }
    }
}

void scanAutomat(int &n, int &m, int &q0, vector<vector<int> > &delta,
                 vector<vector<string> > &f, int &max_l) {
    m = 2;
    cin >> n;
    delta.resize(n, vector<int>(m));
    f.resize(n, vector<string>(m));

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

    cin >> q0 >> max_l;
}

unordered_set<string> generateLanguage(int numQ, int numIn, int startQ, vector<vector<int> > &QInToQ,
                             vector<vector<string> > &QInToOut, int maxLen) {
    unordered_set<string> language;
    unordered_map<State, bool> memo;
    unordered_set<Edge> lambdaQ;

    recursiveGenerate(0, "", startQ, QInToQ, QInToOut, maxLen, language, memo, lambdaQ);

    return language;
}

int main() {
    int n, m, q0, max_l;
    vector<vector<int> > delta;
    vector<vector<string> > f;
    scanAutomat(n, m, q0, delta, f, max_l);
    for (const auto &word: generateLanguage(n, m, q0, delta, f, max_l)) {
        cout << word << endl;
    }
    return 0;
}
