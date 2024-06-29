#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

const int INF = 100000000;
const int dx[] = {-1, 1, 0, 0};
const int dy[] = {0, 0, -1, 1};

int deijstra(vector<vector<int> > map, int N) {
    vector<vector<int> > dist(N, vector<int>(N, INF));
    dist[0][0] = map[0][0];

    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int> >, greater<tuple<int, int, int> > > pq;
    pq.emplace(dist[0][0], 0, 0);

    while (!pq.empty()) {
        auto [d, x, y] = pq.top();
        pq.pop();

        if (d > dist[x][y]) continue;

        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && nx < N && ny >= 0 && ny < N) {
                int new_dist = dist[x][y] + map[nx][ny];
                if (new_dist < dist[nx][ny]) {
                    dist[nx][ny] = new_dist;
                    pq.emplace(new_dist, nx, ny);
                }
            }
        }
    }

    return dist[N - 1][N - 1];
}

int main() {
    int N;
    cin >> N;

    vector<vector<int> > map(N, vector<int>(N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> map[i][j];
        }
    }
    cout << deijstra(map, N) << endl;
    return 0;
}
