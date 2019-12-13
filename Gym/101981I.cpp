#include <bits/stdc++.h>

using namespace std;

#define V 1004 //500 heroes + 500 mons + 4 source/sinks
#define E 251502 // 2 + (2 * 500) + (500 * 500) + 500
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

    fill(start, start + V, -1);
    int n_heroes, m, k; cin >> n_heroes >> m >> k;

    s = n_heroes + m;
    int s_normal = n_heroes + m + 1;
    int s_potion = n_heroes + m + 2;
    t = n_heroes + m + 3;

    //super source to sources
    add_edge(s, s_normal, n_heroes);
    add_edge(s, s_potion, k);

    // sources to heroes
    for(int i = 0; i < n_heroes; ++i) {
        add_edge(s_normal, i, 1);
        add_edge(s_potion, i, 1);
    }

    // monsters to kills
    for(int i = n_heroes; i <  n_heroes + m; ++i) {
        add_edge(i, t, 1);
    }

    // heroes to monsters
    for(int hero = 0; hero < n_heroes; ++hero) {
        int n_monsters; cin >> n_monsters;
        for(int j = 0; j < n_monsters; ++j) {
            int mon; cin >> mon;
            add_edge(hero, n_heroes + mon - 1, 1);
        }
    }

    n = 4 + n_heroes + m;

    cout << mf() << "\n";
    
    return 0;
}