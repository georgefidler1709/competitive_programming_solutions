#include <bits/stdc++.h>

using namespace std;

constexpr int MAX_N = 200000;
constexpr int LOG_N = 18;

using ll = int64_t;

struct edge {
    int u, v, w;
};
bool operator < ( const edge& a, const edge & b) {
    return a.w < b.w;
}
vector<edge> edges;

struct edgeEquality {
    bool operator()(const edge& a, const edge &b) {
        if(a.u == b.u) {
            if(a.v == b.v) {
                return a.w < b.w;
            } else return a.v < b.v;

        } else return a.u < b.u;
    }
};

map<pair<int, int>, int> og_mst_edges;
int parent[MAX_N][LOG_N];
vector<int> depth;
int max_in_path[MAX_N][LOG_N];

void init_uf (int n, vector<int>& parent, vector<int>& subtree_size) {
    for(int i = 0; i < n; i ++) {
        parent[i] = i; subtree_size[i] = 1;
    }
}
int root(int x, vector<int>& parent) {
// only roots are their own parents
// otherwise apply path compression
    return parent [x] == x ? x : parent[x] = root(parent[x], parent) ;
}
void join(int x, int y, vector<int>& parent, vector<int>& subtree_size) {
// size heuristic
// hang smaller subtree under root of larger subtree
    x = root(x, parent); y = root(y, parent);
    if (x == y) return ;
    if ( subtree_size [x] < subtree_size [y]) {
        parent[x] = y;
        subtree_size [y] += subtree_size [x];
    } else {
        parent[y] = x;
        subtree_size [x] += subtree_size [y];
    }
}


ll mst(int nE, vector<int>& parent, vector<int>& subtree_size) {
    ll total_weight = 0;
    for (int i = 0; i < nE; i ++) {
        edge& e = edges[i];
        if(root(e.u, parent) != root(e.v, parent)) {
            total_weight += e.w;
            og_mst_edges.insert({{e.u, e.v}, e.w});
            og_mst_edges.insert({{e.v, e.u}, e.w});
            join(e.u, e.v, parent, subtree_size);
        }
    }
    return total_weight;
}

/*
 * lca 1
 */

// parent[u][k] is the 2^k-th parent of u
void preprocess(int n) {
    for (int i = 0; i < n; i++) {
        // assume parent[i][0] (the parent of i) is already filled in
        for (int j = 1; (1<<j) < n; j++) {
            parent[i][j] = -1;
        }
    }

    // fill in the parent for each power of two up to n
    for (int j = 1; (1<<j) < n; j++) {
        for (int i = 0; i < n; i++) {
            if (parent[i][j-1] != -1) {
                // the 2^j-th parent is the 2^(j-1)-th parent of the 2^(j-1)-th parent
                parent[i][j] = parent[parent[i][j-1]][j-1];
                max_in_path[i][j] = max(max_in_path[i][j-1], max_in_path[parent[i][j-1]][j-1]);
            }
        }
    }
}


/*
 * lca 2
 */

int lca (int u, int v) {
    int max = -1;
    // make sure u is deeper than v
    if (depth[u] < depth[v]) swap(u,v);

    // log[i] holds the largest k such that 2^k <= i
    for (int i = (int)log2(depth[u]); i >= 0; i--) {
        // repeatedly raise u by the largest possible power of two until it is the same depth as v
        if (depth[u] - (1<<i) >= depth[v]) {
            if(max_in_path[u][i] > max) max = max_in_path[u][i];
            u = parent[u][i];
        }

    }

   if (u == v) return max;

    for (int i = (int)log2(depth[u]); i >= 0; i--)
        if (parent[u][i] != -1 && parent[u][i] != parent[v][i]) {
            // raise u and v as much as possible without having them coincide
            // this is important because we're looking for the lowest common ancestor, not just any
            if(max_in_path[u][i] > max) max = max_in_path[u][i];
            if(max_in_path[v][i] > max) max = max_in_path[v][i];
            u = parent[u][i];
            v = parent[v][i];
        }

    // u and v are now distinct but have the same parent, and that parent is the LCA
    max = (max > max_in_path[u][0]) ? max : max_in_path[u][0];
    max = (max > max_in_path[v][0]) ? max : max_in_path[v][0];
    return max;
    // return parent[u][0];
}




/* Define Infinite as a large enough
value.This value will be used for
vertices not connected to each other */
constexpr ll INF = 1000000001;

void dfs(vector<vector<int>>& graph, int root = 0, int d = 0) {
    depth[root] = d;
    for (int i = 0; i < graph[root].size(); i++) {
        int des = graph[root][i];
        if (depth[des] <= -1) {
            parent[des][0] = root;
            max_in_path[des][0] = og_mst_edges.find({root, des})->second;
            dfs(graph, des, d + 1);
        }
    }
}

int main() {
    int nV, nE;
    cin >> nV >> nE;

    if(nE == 0 || nV == 0) {
        return 0;
    }

    for(int i = 0; i < nE; ++i) {
        int u, v;
        int w;
        cin >> u >> v >> w;
        --u, --v;
        edges.push_back({u,v,w});
    }

    vector<edge> edges_in_order = edges;

    sort(edges.begin(), edges.end());
    ll og_mst_weight;
    {
        vector<int> og_parent(nE);
        vector<int> og_subtree_size(nE);
        init_uf(nV, og_parent, og_subtree_size);
        og_mst_weight = mst(nE, og_parent, og_subtree_size);
    }

    depth = vector<int>(nV, -1);

    {
        vector<vector<int>>graph(nV, vector<int>());
        for(const auto& e : og_mst_edges) {
            graph[e.first.first].push_back(e.first.second);
        }
        dfs(graph);
    }

    preprocess(nV);

    for(edge ed : edges_in_order) {
        if(og_mst_edges.find({ed.u, ed.v}) != og_mst_edges.end()) cout << og_mst_weight << "\n";
        else {
            int max = lca(ed.u, ed.v);
            cout << og_mst_weight - (ll)max + (ll)ed.w << "\n";
        }
    }


}

