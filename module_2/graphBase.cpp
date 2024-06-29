#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

void dfsTarjan(int node, int &timer, vector<int> &discoveryTime, vector<int> &lowestTime,
    stack<int> &stk, vector<bool> &inStack, vector<vector<int>> &adjList,
    vector<vector<int>> &stronglyConnectedComponents) {
    discoveryTime[node] = lowestTime[node] = ++timer;
    stk.push(node);
    inStack[node] = true;

    for (int neighbor : adjList[node]) {
        if (discoveryTime[neighbor] == -1) {
            dfsTarjan(neighbor, timer, discoveryTime, lowestTime, stk,
                inStack, adjList, stronglyConnectedComponents);
            lowestTime[node] = min(lowestTime[node], lowestTime[neighbor]);
        } else if (inStack[neighbor]) {
            lowestTime[node] = min(lowestTime[node], discoveryTime[neighbor]);
        }
    }

    if (lowestTime[node] == discoveryTime[node]) {
        vector<int> component;
        while (stk.top() != node) {
            int topNode = stk.top();
            stk.pop();
            component.push_back(topNode);
            inStack[topNode] = false;
        }
        int topNode = stk.top();
        stk.pop();
        component.push_back(topNode);
        inStack[topNode] = false;
        stronglyConnectedComponents.push_back(component);
    }
}

vector<vector<int>> findStronglyConnectedComponents(int nodes, vector<vector<int>> &adjList) {
    vector<int> discoveryTime(nodes, -1), lowestTime(nodes, -1);
    vector<bool> inStack(nodes, false);
    stack<int> stk;
    vector<vector<int>> stronglyConnectedComponents;

    int timer = 0;
    for (int i = 0; i < nodes; i++) {
        if (discoveryTime[i] == -1) {
            dfsTarjan(i, timer, discoveryTime, lowestTime, stk,
                inStack, adjList, stronglyConnectedComponents);
        }
    }

    return stronglyConnectedComponents;
}

vector<int> findBaseVertices(vector<vector<int>> &adjList,
    vector<vector<int>> &stronglyConnectedComponents) {
    int nodes = adjList.size();
    int componentCount = stronglyConnectedComponents.size();
    vector<int> componentId(nodes, -1);
    vector<vector<int>> condensedGraph(componentCount);

    for (int i = 0; i < componentCount; i++) {
        for (int node : stronglyConnectedComponents[i]) {
            componentId[node] = i;
        }
    }

    vector<int> inDegree(componentCount, 0);
    for (int u = 0; u < nodes; u++) {
        for (int v : adjList[u]) {
            if (componentId[u] != componentId[v]) {
                condensedGraph[componentId[u]].push_back(componentId[v]);
                inDegree[componentId[v]]++;
            }
        }
    }

    vector<int> baseVertices;
    for (int i = 0; i < componentCount; i++) {
        if (inDegree[i] == 0) {
            int minVertex = *min_element(stronglyConnectedComponents[i].begin(),
                stronglyConnectedComponents[i].end());
            baseVertices.push_back(minVertex);
        }
    }

    sort(baseVertices.begin(), baseVertices.end());
    return baseVertices;
}

int main() {
    int numNodes, numEdges;
    cin >> numNodes >> numEdges;
    vector<vector<int>> adjList(numNodes);

    for (int i = 0; i < numEdges; i++) {
        int from, to;
        cin >> from >> to;
        adjList[from].push_back(to);
    }

    vector<vector<int>> stronglyConnectedComponents = findStronglyConnectedComponents(numNodes, adjList);
    vector<int> baseVertices = findBaseVertices(adjList, stronglyConnectedComponents);

    for (int vertex : baseVertices) {
        cout << vertex << " ";
    }
    cout << endl;

    return 0;
}
