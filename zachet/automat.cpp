#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

struct Automaton {
    int M, N, q0;
    vector<bool> is_final;
    vector<vector<int>> transitions;
};

Automaton read_automaton(int M) {
    Automaton A;
    A.M = M;
    cin >> A.N >> A.q0;
    A.is_final.resize(A.N);
    A.transitions.resize(A.N, vector<int>(A.M));

    for (int i = 0; i < A.N; ++i) {
        char type;
        cin >> type;
        A.is_final[i] = (type == '+');
        for (int j = 0; j < A.M; ++j) {
            cin >> A.transitions[i][j];
        }
    }

    return A;
}

string find_shortest_distinguishing_string(const Automaton& A1, const Automaton& A2) {
    queue<tuple<int, int, string>> q;
    vector<vector<bool>> visited(A1.N, vector<bool>(A2.N, false));

    q.push(make_tuple(A1.q0, A2.q0, ""));
    visited[A1.q0][A2.q0] = true;

    while (!q.empty()) {
        auto [state1, state2, str] = q.front();
        q.pop();

        if (A1.is_final[state1] != A2.is_final[state2]) {
            return str;
        }

        for (int i = 0; i < A1.M; ++i) {
            int next_state1 = A1.transitions[state1][i];
            int next_state2 = A2.transitions[state2][i];
            if (!visited[next_state1][next_state2]) {
                q.push(make_tuple(next_state1, next_state2, str + char('a' + i)));
                visited[next_state1][next_state2] = true;
            }
        }
    }

    return "=";
}

int main() {
    int M;
    cin >> M;

    Automaton A1 = read_automaton(M);
    Automaton A2 = read_automaton(M);

    string result = find_shortest_distinguishing_string(A1, A2);
    if (result=="=") {
        cout << result << endl;
        return 0;
    }
    cout << result.size()<<endl;
    return 0;
}
