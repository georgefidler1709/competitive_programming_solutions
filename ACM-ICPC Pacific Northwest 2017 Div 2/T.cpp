#include <bits/stdc++.h>

using namespace std;

#define MAXN 1005
#define MAXM 5005
#define MAXK 1000000005

struct door {
    int dst, l, r;
};


vector<door> adj[MAXN];

/*
 * dfs
 */

// global arrays are initialised to zero for you
bool seen[MAXN];

// modified dfs to only accept edges with u within their range
void dfs(int u, int id) {
    if (seen[u]) return;
    seen[u] = true;
    for (const auto& v : adj[u]) {
        if(v.l <= id && v.r >= id) {
            dfs(v.dst, id);
        }
    }
}



int main() {
    int n, m, k; cin >> n >> m >> k;
    int s, t; cin >> s >> t;

    set<int> edge_ends;

    for(int i = 0; i < m; ++i) {
        int a, b, c, d; cin >> a >> b >> c >> d;
        adj[a].push_back({b, c, d});

        // sort range start/end points
        edge_ends.insert(c);
        // end points incremented bc ranges are inclusive
        // and we want each to represent the start of a new range
        edge_ends.insert(d + 1);
    }

    int total = 0;
    int prev = -1;
    for(auto cur : edge_ends) {
        // if the prev range start/end point was able to reach the destination
        // all id's in the range [prev, cur] can
        if(prev != -1) total += cur - prev;
        fill(seen, seen + MAXN, false);
        dfs(s, cur);
        prev = seen[t] ? cur : -1;
    }

    cout << total << "\n";
}

