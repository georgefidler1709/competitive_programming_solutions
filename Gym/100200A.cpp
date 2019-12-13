// Min cut is unique on this condition:
// (nodes reachable from s) + (nodes that can reach t) == all nodes
// If not then there is a saturated node not part of the min cut i.e. it could be part of the min cut

#include <bits/stdc++.h>

using namespace std;

#define V 1500
#define E 251502
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

bool seen[V];

void check_reach(int u) {
    if (seen[u]) return;
    seen[u] = true;
    for (int e = start[u]; ~e; e = succ[e]) {
        if (cap[e] > 0) check_reach(to[e]);
    }
}

int inv(int e) { return e ^ 1; }

vector<int> min_cut(int edges) {
    vector<int> ans;
    fill(seen, seen + V, 0);
    check_reach(s);
    // the odd-numbered edges are the residual ones
    for (int e = 0; e < edges; e += 2) {
        if (!seen[to[e]] && seen[to[inv(e)]]) {
            ans.push_back(e);
        }
    }
    return ans;
}


int main() {
    ifstream cin("attack.in");
    ofstream cout("attack.out");

    fill(start, start + V, -1);
    int num_n, m, a, b; cin >> num_n >> m >> a >> b;
    --a; --b;

    int edge_weights[2 * E];
    for(int i  = 0; i < m; ++i) {
        int u, v, c; cin >> u >> v >> c;
        --u; --v;

        if(u != b && v != a) {
            edge_weights[edge_counter] = c;
            add_edge(u, v, c);
        }
        if(u != a && v != b) {
            edge_weights[edge_counter] = c;
            add_edge(v, u, c);
        }
    }

    s = a;
    t = b;
    n = num_n;

    int first_flow = mf();

    fill(seen, seen + V, 0);
    check_reach(a);

    bool total_seen[V];
    for(int i = 0; i < n; ++i) total_seen[i] = seen[i];

    for(int i = 0; i < n; ++i) {
        if(total_seen[i]) continue;
        fill(seen, seen + V, 0);
        check_reach(i);
        if(!seen[b]) {
            cout << "AMBIGUOUS\n";
            return 0;
        }

    }

    cout << "UNIQUE\n";
}
