#include <bits/stdc++.h>

using namespace std;

constexpr int MAXN = 400;
constexpr int INF = 100000;
using ll = long long;

struct edge {
    int u, v, w;
    edge() {}
    edge(int nu, int nv, int nw) : u(nu), v(nv), w(nw) {}
};

edge edges[5 * MAXN];

ll x1[MAXN], x2[MAXN], seg_ends[2 * MAXN], dist[2 * MAXN];
map<ll, ll> ordered_ends;

/*
 * bellman ford relax
 */

// global relaxation: try to relax every edge in the graph
// Returns if any distance was updated.
bool relax(int n) {
    bool relaxed = false;
    for (int i = 0; i < n; i++) {
        auto e = edges[i];
        // we don't want to relax an edge from an unreachable vertex
        if (dist[e.v] > dist[e.u] + e.w) {
            relaxed = true;
            dist[e.v] = dist[e.u] + e.w;
        }
    }
    return relaxed;
}

/*
 * bellman ford main
 */

// Puts distances from source (n-1) in dist
// Returns true if there is a negative cycle, false otherwise.
// NOTE: You can't trust the dist array if this function returns True.
bool find_dists_and_check_neg_cycle(int n, int n_edges) {
    fill(dist, dist + n, INF);
    dist[0] = 0;
    for (int i = 1; i < n; i++) relax(n_edges);
    return relax(n_edges);
}

int main() {
    int n; cin >> n;
    int n_ends = 0;
    for (int i = 0; i < n; i++) {
        int h; //height is useless but we have to put it somewhere
        cin >> x1[i] >> x2[i] >> h;

        // keeps 0 but bumps up the others to allow y to always be > 0
        x1[i] = x1[i] << 1;
        x2[i] = (x2[i] << 1) - 1;
        seg_ends[n_ends++] = x1[i];
        seg_ends[n_ends++] = x2[i];
    }
    sort(seg_ends, seg_ends + n_ends);
    // remove dupes
    n_ends = (int)(unique(seg_ends, seg_ends + n_ends) - seg_ends);
    int n_edges = n;

    ordered_ends[seg_ends[0]] = 0;
    // constraint: S_{i} - S_{i - 1} ≥ 0
    for(int i = 1; i < n_ends; i++) {
        ordered_ends[seg_ends[i]] = i;
        edges[n_edges++] = edge(i, i - 1, 0);
    }

    // stop back edges
    for(int i = 0; i < n; i++) edges[n_edges++] = edge(ordered_ends[x2[i]], ordered_ends[x1[i]], -1);

    // binary search through all possible values of k to find the lowest
    int lo = 0, hi = n, k = hi;
    while(lo <= hi) {
        int mid = lo + hi >> 1;
        // constraint: S_{ri} - S_{li - 1} ≤ K
        for (int i = 0; i < n; i++) edges[i] = edge(ordered_ends[x1[i]], ordered_ends[x2[i]], mid);
        // bellman ford to solve system of linear equations:
        // https://courses.csail.mit.edu/6.006/spring11/lectures/lec17.pdf?fbclid=IwAR0n0z0YxvFBCvWVFZfeBvvEuQo4boSd_amK4420rjXUU4xruKiea2-iMkI
        if(!find_dists_and_check_neg_cycle(n_ends, n_edges)) k = mid, hi = mid - 1;
        else lo = mid + 1;
    }
    cout << k << "\n";
}