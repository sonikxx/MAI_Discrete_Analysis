#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

void dfs(vector<vector<int64_t>> &graph, vector<int64_t> &check, int64_t i, int64_t start_index) {
    check[i] = start_index;
    for (int64_t k = 0; k < graph[i].size(); ++k) {
        if (check[graph[i][k]] == -1) {
            dfs(graph, check, graph[i][k], start_index);
        }
    }
}

void bfs(vector<vector<int64_t>> &graph, vector<int64_t> &check, int64_t i, int64_t component_number) {
    queue<int64_t> babube;
    babube.push(i);
    check[i] = component_number;
    while (!babube.empty()) {
        int64_t tmp = babube.front();
        babube.pop();
        for (int64_t j = 0; j < graph[tmp].size(); ++j) {
            if (check[graph[tmp][j]] == -1) {
                babube.push(graph[tmp][j]);
                check[graph[tmp][j]] = component_number;
            }
        }
    }
}

int main() {
    int64_t n, m;
    cin >> n >> m;
    vector<vector<int64_t>> graph(n);
    vector<int64_t> check(n, -1);
    for (int64_t i = 0; i < m; ++i) {
        int64_t u, v;
        cin >> u >> v;
        graph[u - 1].push_back(v - 1);
        graph[v - 1].push_back(u - 1);
    }
    int64_t counter = 0;
    for (int i = 0; i < n; ++i) {
        if (check[i] == -1) {
            bfs(graph, check, i, counter);
            ++counter;
        }
    }
    vector<vector<int64_t>> result(counter);
    for (int64_t i = 0; i < n; ++i) {
        result[check[i]].push_back(i);
    }
    // for (int64_t i = 0; i < counter; ++i) {
    //     sort(result[i].begin(), result[i].end());
    // }
    // sort(result.begin(), result.end());
    // for (int64_t i = 0; i < result.size(); ++i) {
    //     for (int j = 0; j < result[i].size(); ++j) {
    //         cout << result[i][j] + 1 << " ";
    //     }
    //     cout << endl;
    // }
    return 0;
}