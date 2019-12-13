#include <bits/stdc++.h>

using namespace std;

using ll = long long;

#define V 2000
#define E 10000 // no limit given??
#define INF INT_MAX
#define ITERATIONS 200

/*
 * edge list representation
 */

// the index of the first outgoing edge for each vertex, initialised to -1
ll start[V];
// if e is an outgoing edge from u, succ[e] is another one, or -1
// cap[e] is the capacity/weight of the e
// to[e] is the destination vertex of e
ll succ[E], to[E];
double cap[E];

ll edge_counter = 0;
void add_single_edge(ll u, ll v, double c) {
    // set the properties of the new edge
    cap[edge_counter] = c, to[edge_counter] = v;
    // insert this edge at the start of u's linked list of edges
    succ[edge_counter] = start[u];
    start[u] = edge_counter;
    ++edge_counter;
}

void add_edge(ll u, ll v, double c) {
    add_single_edge(u, v, c); // original
    add_single_edge(v, u, 0); // residual edge

}

/*
 * dinics 1
 */

// assumes the residual graph is constructed as in the previous section
// n = #nodes, s = source, t = sink
ll n, s, t;
// stores dist from s.
ll lvl[V];
// stores first non-useless child.
ll nextchld[V];

// constructs the BFS tree.
// Returns if the sink is still reachable.
bool bfs() {

    for (ll i = 0; i < n; i++) lvl[i] = -1;
    queue<ll> q;
    q.push(s); lvl[s] = 0;
    while (!q.empty()) {
        ll u = q.front(); q.pop();
        nextchld[u] = start[u]; // reset nextchld
        for (ll e = start[u]; ~e; e = succ[e]) {
            if (cap[e] > 0) {
                ll nxt = to[e];
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
double aug(ll u, double cflow) {
    if (u == t) return cflow; // base case.
    // Note the reference here! We will keep decreasing nextchld[u]
    // Till we find a child that we can flow through.
    for (ll &i = nextchld[u]; i >= 0; i = succ[i]) {
        if (cap[i] > 0) {
            ll nxt = to[i];
            // Ignore edges not in the BFS tree.
            if (lvl[nxt] != lvl[u] + 1) continue;
            double rf = aug(nxt, min(cflow, cap[i]));
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

double mf() {
    double tot = 0;
    while (bfs())
        for (double x = aug(s,INF); x > 0; x = aug(s,INF)) tot+=x;
    return tot;
}


bool seen[V];

void check_reach(int u) {
    if (seen[u]) return;
    seen[u] = true;
    for (ll e = start[u]; ~e; e = succ[e]) {
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


pair<ll,ll> og_edges[E];
ll final_layer_start = 0;

double run_max_flow(int n_nodes, int n_edges, double h) {

    edge_counter = 0;
    fill(start, start + V, -1);

    // add edges from source to 'edges'
    for(ll i = 1; i <= n_edges; ++i) {
        add_edge(s, i, 1);
    }

    // add edges from 'edges' to 'nodes'
    for(ll i = 1; i <= n_edges; ++i) {
        auto e = og_edges[i - 1];
        add_edge(i, n_edges + e.first, INF);
        add_edge(i, n_edges + e.second, INF);
    }

    final_layer_start = edge_counter;
    //add edges from 'nodes' to sink
    for(ll i = 1; i <= n_nodes; ++i) {
        add_edge(n_edges + i, t, h);
    }

    return mf();
}


double  binarysearch(ll n_nodes, ll n_edges) {
    double lo = 0;
    double hi = n_edges;
    double mid = -1;
    // Range [lo, hi];
    for(int j = 0; j < ITERATIONS; ++j) {
        mid = (lo + hi) / 2;
        double res = run_max_flow(n_nodes, n_edges, mid);
        if (n_edges - res <= 1e-6) {
            hi = mid;
        } else {
            lo = mid;
        }
    }
    return mid;
}

int main() {
//    ios_base::sync_with_stdio(false);
//    cin.tie(NULL);

    ll n_nodes, n_edges;
    while(cin >> n_nodes >> n_edges) {
        if(n_edges == 0) {
            cout << "1\n1\n\n";
            continue;
        }

        for(ll i = 0; i < n_edges; ++i) {
            ll u, v; cin >> u >> v;
            og_edges[i] = {u,v};
        }

        s = 0;
        t = n_edges + n_nodes + 1;
        n = n_edges + n_nodes + 2;

        double h = binarysearch(n_nodes, n_edges);
        auto cut = min_cut(edge_counter);

        vector<ll>hires;
        for(auto e : cut) {
            if(e >= final_layer_start) {
                hires.push_back((e - final_layer_start)/2 + 1);
            }
        }

        cout << hires.size() << "\n";
        for(auto hire : hires) cout << hire << "\n";
        cout << "\n";
    }

}