#include <iostream>
#include <vector>
using namespace std;

int max(int a, int b) {
    if (a>=b) {
        return a;
    }
    return b;
}

int min(int a, int b) {
    if (a<b) {
        return a;
    }
    return b;
}

struct edge {
    int a, b, cost;
};

int main() {
    int n,k,c;

    cin >> n;
    vector<edge> graph;
    for (int i=0;i<n;i++) {
        cin >> k;
        for (int j=0;j<k;j++) {
            cin >> c;
            edge e;
            e.a=c-1;
            e.b=i;
            e.cost=1;
            graph.push_back(e);
        }
    }
    const int inf = -100000000;
    vector<vector<int>> d(n,vector<int>(n,inf));

    for (int i=0;i<n;i++) {
        d[i][i]=0;
    }
    int size = graph.size();
    for (int k=0;k<n;k++) {
        for (int i=0;i<n-1;++i) {
            for (int j=0;j<size;++j) {
                d[k][graph[j].b]=max(d[k][graph[j].b],d[k][graph[j].a]+graph[j].cost);

            }
        }
    }
 /*   for (vector<int> el: d) {
        for (int e: el) {
            cout << e << " ";
        }
        cout << endl;
    }*/
    int maxi = -1;
    for (int i=0;i<n;i++) {
        for (int j=0;j<n;j++) {
            if (i!=j && d[i][j]>=0 && d[j][i]>=0) {
                cout << "cycle" << endl;
                return 0;
            }
            if (d[i][j]!=inf) {
                maxi = max(maxi, d[i][j]+1);
            }
        }
    }
    cout << maxi << endl;
    return 0;
}
