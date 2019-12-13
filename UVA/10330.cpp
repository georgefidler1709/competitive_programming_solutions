#include <bits/stdc++.h>

using namespace std;

#define V 500
#define E 100000 // no limit given??
#define INF INT_MAX

/*
 * edge list representation
 */

// the index of the first outgoing edge for each vertex, initialised to -1
int start[V];
// if e is an outgoing edge from u, succ[e] is another one, or -1
// cap[e] is the capacity/weight of the e
// to[e] is the destination vertex of e
int succ[E], cap[E], to[E];

int edge_counter = 0;
void add_single_edge(int u, int v, int c) {
    // set the properties of the new edge
    cap[edge_counter] = c, to[edge_counter] = v;
    // insert this edge at the start of u's linked list of edges
    succ[edge_counter] = start[u];
    start[u] = edge_counter;
    ++edge_counter;
}

void add_edge(int u, int v, int c) {
    add_single_edge(u, v, c); // original
    add_single_edge(v, u, 0); // residual edge

}

/*
 * dinics 1
 */

// assumes the residual graph is constructed as in the previous section
// n = #nodes, s = source, t = sink
int n, s, t;
// stores dist from s.
int lvl[V];
// stores first non-useless child.
int nextchld[V];

// constructs the BFS tree.
// Returns if the sink is still reachable.
bool bfs() {

    for (int i = 0; i < n; i++) lvl[i] = -1;
    queue<int> q;
    q.push(s); lvl[s] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        nextchld[u] = start[u]; // reset nextchld
        for (int e = start[u]; ~e; e = succ[e]) {
            if (cap[e] > 0) {
                int nxt = to[e];
                if (lvl[nxt] != -1) continue; // already seen
                lvl[nxt] = lvl[u] + 1;
                q.push(nxt);
            }
        }
    }
    return lvl[t] != -1;
}

/*
 * dinics 2
 */

// Finds an augmenting path with up to cflow flow.
int aug(int u, int cflow) {
    if (u == t) return cflow; // base case.
    // Note the reference here! We will keep decreasing nextchld[u]
    // Till we find a child that we can flow through.
    for (int &i = nextchld[u]; i >= 0; i = succ[i]) {
        if (cap[i] > 0) {
            int nxt = to[i];
            // Ignore edges not in the BFS tree.
            if (lvl[nxt] != lvl[u] + 1) continue;
            int rf = aug(nxt, min(cflow, cap[i]));
            // Found a child we can flow through!
            if (rf > 0) {
                cap[i] -= rf;
                cap[i^1] += rf;
                return rf;
            }
        }
    }
    lvl[u]=-1;
    return 0;
}

int mf() {
    int tot = 0;
    while (bfs())
        for (int x = aug(s,INF); x; x = aug(s,INF)) tot+=x;
    return tot;
}

int main() {
    int n_regs;

    while(cin >> n_regs) {
        fill(start, start + V, -1);
        // adding capacities of regulators
        for(int i = 1; i <=  n_regs; ++i) {
            int c; cin >> c;
            // node capacity so split in 2
            add_edge(i, n_regs + i, c);
        }

        // adding connections between reglulators
        int m; cin >> m;
        for(int i = 1; i <= m; ++i) {
            int u, v, c; cin >> u >> v >> c;
            // edge goes from the end of 'edge' u to beginning of 'edge' v
            add_edge(u + n_regs, v, c);
        }

        // connecting sources/sinks to supers
        int b, d; cin >> b >> d;
        s = 0;
        t = 2 * n_regs + 1;
        n = 2 * n_regs + 2;
        for(int i = 0; i < b; ++i) {
            int ss; cin >> ss;
            add_edge(s, ss, INF);
        }

        for(int i = 0; i < d; ++i) {
            int st; cin >> st;
            add_edge(st + n_regs, t, INF);
        }

        cout << mf() << "\n";
    }



}
