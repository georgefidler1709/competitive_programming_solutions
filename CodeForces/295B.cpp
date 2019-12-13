#include <bits/stdc++.h>

using namespace std;

constexpr int MAX_N = 500;

using ll = long long;

int dist[MAX_N][MAX_N];

void floyd(int n, const vector<vector<int>>& adj, vector<ll>& dist_totals) {
    // the distance between everything is infinity
    for (int u = 0; u < n; ++u) {
        for (int v = 0; v < n; ++v) {
            dist[u][v] = INT_MAX;
        }
    }

// update the distances for every directed edge
    for(int u = 0; u < n; ++u) {
        for(int v = 0; v < n; ++v) {
            dist[u][v] = adj[u][v];
        }
    }


// every vertex can reach itself
    for (int u = 0; u < n; ++u) dist[u][u] = 0;

    for (int i = 0; i < n; i++) {
        for (int u = 0; u < n; u++) {
            for (int v = 0; v < n; v++) {
                // dist[u][v] is the length of the shortest path from u to v using only 0 to i-1 as intermediate vertices
                // now that we're allowed to also use i, the only new path that could be shorter is u -> i -> v
                dist[u][v] = min(dist[u][v], dist[u][i] + dist[i][v]);
            }
        }
        // here we want to find all edge weights relevant to the current set, sum them and add them to some stored set to reverse and then print out
        for(int u = 0; u <= i; ++u) {
            for(int v = 0; v <= i; ++v) {
                dist_totals[i] += dist[u][v];
            }
        }
    }
}

int main() {

    int n;
    cin >> n;

    vector<vector<int>>full_adj(n, vector<int>(n));
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            cin >> full_adj[i][j];
        }
    }

    vector<int>node_insertions(n);
    for(int i = 0; i < n; ++i) {
        int node;
        cin >> node;
        --node;
        node_insertions[i] = node;
    }

    reverse(node_insertions.begin(), node_insertions.end());

    map<int, int>order_to_node;
    for(int i = 0; i < n; ++i) {
        order_to_node[i] = node_insertions[i];
    }

    vector<vector<int>>ordered_adj(n, vector<int>(n));
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            ordered_adj[i][j] = full_adj[order_to_node.find(i)->second][order_to_node.find(j)->second];
        }
    }
    vector<ll> dist_totals(n,0);
    floyd(n, ordered_adj, dist_totals);
    for(auto it = dist_totals.crbegin(); it != dist_totals.crend(); ++it) {
        cout << *it << " ";
    }

}