// 1. Modify bellman-ford to keep track of reachability. If we can reach dst from src we are done
// 2. Have bellman-ford also keep track of nodes in positive cycles.
// 3. Try to reach dst (without caring about energy) from each of these nodes.

// Floyd-Warshall does negative cycle finding!
// Convert that to positive cycle finding and then mark any node in the cycle as INF energy
// Can't use my method bc if cycles are within cycles it isn't guaranteed to find all cycles (positive cycle may be hidden by negative cycle)

// NOTE: Just getting to the exit with a positive total is not enough, need to stop if you hit 0 at any point along the way
#include <bits/stdc++.h>

using namespace std;

constexpr int MAX_ROOMS = 100;
constexpr int ENERGY_START = 100;

bool bfs_pos_cycles(int V, int exit, vector<vector<int>>& adj, set<int>& pos_cycle_nodes) {
    queue<int> q;
    vector<bool>visited(V, false);
    for(auto n : pos_cycle_nodes) {
        q.push(n);
        visited[n] = true;
    }



    while(!q.empty()) {
        auto u = q.front();
        if(u == exit) return true;
        q.pop();
        for (int v : adj[u]) {
            if(!visited[v]) {
                visited[u] = true;
                q.push(v);
            }
        }
    }

    return false;

}

bool isPosCycleBellmanFord(int V, vector<vector<int>>& adj, vector<int>& room_effect, set<int>& pos_cycle_nodes, int src = 0) {

    vector<int> energy(V, INT_MAX);
    energy[src] = -ENERGY_START;
    vector<int>round_energy = energy;

    vector<bool>reachable(V, false);
    reachable[src] = true;
    vector<bool>round_reachable = reachable;

    for (int i = 1; i <= 2 * V; i++) {
        for (int u = 0; u < V; u++) {
            for (int v : adj[u]) {
                int e = room_effect[v];
                int dst_energy = energy[u] + e;

                if (dst_energy < 0) {
                    if (reachable[u]) {
                        if (!reachable[v] || dst_energy < energy[v]) {
                            round_energy[v] = dst_energy;
                            round_reachable[v] = true;
                        }
                    }
                }
            }
        }

        if (round_reachable[V - 1]) {
            return true;
        }

        for (int u = 0; u < V; u++) {
            if (round_reachable[u]) {
                if (reachable[u]) {
                    if (energy[u] > round_energy[u]) {
                        if (i >= V) {
                            pos_cycle_nodes.insert(u);
                        }
                    }
                }
                else {
                    if (i >= V) {
                        pos_cycle_nodes.insert(u);
                    }
                }
            }
        }
        energy = round_energy;
        reachable = round_reachable;
    }

    return false;
}


int main() {
    while(true) {
        int n_rooms;
        cin >> n_rooms;
        if(n_rooms == -1) break;
        set<int> pos_cycle_nodes;
        vector<int>last_seen_e(n_rooms);
        vector<int> room_effect(MAX_ROOMS);
        vector<vector<int>> adj = vector<vector<int>>(n_rooms, vector<int>());
        for(int i = 0; i < n_rooms; ++i) {
            int e;
            // energy from room
            cin >> e;
            room_effect[i] = -e;

            int n_doors;
            cin >> n_doors;

            for(int j = 0; j < n_doors; ++j) {
                int edge;
                cin >> edge;
                --edge;
                adj[i].push_back(edge);
            }
        }

        auto result = isPosCycleBellmanFord(n_rooms, adj, room_effect, pos_cycle_nodes);
        if(! result) {
            result = bfs_pos_cycles(n_rooms, n_rooms - 1, adj, pos_cycle_nodes);
        }

        string out = (result) ? "winnable" : "hopeless";
        cout << out << "\n";
    }

}