#include <bits/stdc++.h>
using ll = long long;
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
ll succ[E], cap[E], to[E];

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
ll aug(int u, ll cflow) {
    if (u == t) return cflow; // base case.
    // Note the reference here! We will keep decreasing nextchld[u]
    // Till we find a child that we can flow through.
    for (int &i = nextchld[u]; i >= 0; i = succ[i]) {
        if (cap[i] > 0) {
            ll nxt = to[i];
            // Ignore edges not in the BFS tree.
            if (lvl[nxt] != lvl[u] + 1) continue;
            ll rf = aug(nxt, min(cflow, cap[i]));
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
        for (ll x = aug(s,INF); x; x = aug(s,INF)) tot+=x;
    return tot;
}

int og_edge_costs[E];

#define MAX_WEIGHT 1000000
#define ITERATIONS 100

double  binarysearch(int m, int x) {
    double lo = 0;
    double hi = MAX_WEIGHT;
    double bestSoFar = -1;
    // Range [lo, hi];
    for(int j = 0; j < ITERATIONS; ++j) {
        double mid = (lo + hi) / 2;

        for(int i = 0; i < m; ++i) cap[2 * i] = ll(og_edge_costs[i] / mid);
        int res = mf();
        if (res >= x) {
            bestSoFar = mid;
            lo = mid;
        } else {
            hi = mid;
        }
    }
    return bestSoFar;
}
int main() {
    fill(start, start + V, -1);

    int n_nodes, m, x; cin >> n_nodes >> m >> x;

    for(int i = 0; i < m; ++i) {
        int u, v, c; cin >> u >> v >> c;
        u--; v--;
        og_edge_costs[i] = c;
        add_edge(u, v, c);
    }

    s = 0;
    t = n_nodes - 1;
    n = n_nodes;

    printf("%.10f\n", binarysearch(m , x) * x);
}
