

#include <bits/stdc++.h>

using namespace std;

#define MAXN 200000
#define LOG 18
#define M 400000
#define Q 200000

/*
 * union find
 */

int parent_wolf[MAXN];
int parent_human[MAXN];

int lca_parent_wolf[MAXN][LOG];
int lca_parent_human[MAXN][LOG];


void init(int n, int parent[]) {
    for (int i = 0; i < n; i++) {
        parent[i] = i;
    }
}

int root(int x, int parent[]) {
    // only roots are their own parents
    // otherwise apply path compression
    return parent[x] == x ? x : (parent[x] = root(parent[x], parent));
}

void join(int u, int v, int parent[], int lca_parent[MAXN][LOG], vector<int> children[]) {
    // size heuristic
    // hang smaller subtree under root of larger subtree
    int x = root(v, parent);
    int y = root(u, parent);
    if (x != y) {
        parent[x] = y;
        lca_parent[x][0] = y;
        children[u].push_back(x);
    }
}


/*
 * topological sort
 */

// if the edges are in ASCENDING order of node number,
// this produces the lexicographically GREATEST ordering
int depth_wolf[MAXN];
int depth_human[MAXN];
vector<int> children[MAXN];

// parent[u][k] is the 2^k-th parent of u
void preprocess(int n, int parent[MAXN][LOG]) {
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
            }
        }
    }
}

int highest_ancestor_human(int u, int l, int parent[MAXN][LOG], int depth[]) {
    for (int i = (int) log2(depth[u]); i >= 0; i--) {
        if (parent[u][i] != -1 && parent[u][i] > l) {
            // raise u and v as much as possible without having them coincide
            // this is important because we're looking for the lowest common ancestor, not just any
            u = parent[u][i];
        }
    }
    return parent[u][0] >= l ? parent[u][0] : u;
}

int highest_ancestor_wolf(int u, int r, int parent[MAXN][LOG], int depth[]) {
    for (int i = (int) log2(depth[u]); i >= 0; i--) {
        if (parent[u][i] != -1 && parent[u][i] < r) {
            // raise u and v as much as possible without having them coincide
            // this is important because we're looking for the lowest common ancestor, not just any
            u = parent[u][i];
        }
    }

    return parent[u][0] <= r ? parent[u][0] : u;
}

/*
 * range updates range queries 1
 */

vector<int> tree[4 * MAXN + 5];

void update(int a, int v, int i, int cLeft, int cRight) {
    tree[i].push_back(v);
    if (cRight - cLeft == 1) {
        return;
    }

    int mid = (cLeft + cRight) / 2;
    if (a < mid) update(a, v, i * 2, cLeft, mid);
    else update(a, v, i * 2 + 1, mid, cRight);

}

int query(int qL, int qR, int vL, int vR,  int i, int cLeft, int cRight) {

    while (!tree[i].empty() && tree[i].back() >= vR) tree[i].pop_back();

    if (qL == cLeft && qR == cRight) {
        return !tree[i].empty() && tree[i].back() >= vL;
    }
    int mid = (cLeft + cRight) / 2;
    bool ans = false;
    if (qL < mid) ans = ans || query(qL, min(qR, mid), vL, vR, i * 2, cLeft, mid);
    if (qR > mid && !ans) ans = ans || query(max(qL, mid), qR, vL, vR, i * 2 + 1, mid, cRight);
    return ans;
}

// A node is responsible for the range [startRange[v], endRange[v])
int startRange_human[MAXN+5], endRange_human[MAXN+5];
int startRange_wolf[MAXN+5], endRange_wolf[MAXN+5];

int totId = 0;
void compute_tree_ranges(int c, int startRange[], int endRange[], int depth[], int d = 0) {
    startRange[c] = totId++;
    depth[c] = d;
    for (int nxt : children[c]) {
        compute_tree_ranges(nxt, startRange, endRange, depth, d + 1);
    }
    endRange[c] = totId;
}


struct q_obj{
    int q, s, e, l, r;
};
bool q_sort(const q_obj &a, const q_obj &b) {
    return a.r > b.r;
}

vector<int> adj[MAXN];

int main() {

    int n, m, q; cin >> n >> m >> q;

    for(int i = 0; i < m; ++i) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }


// form the path tree for werewolf
    init(n, parent_wolf);
    for (int u = 0; u < n; u++){
        for (int v : adj[u]) {
            if (v < u) {
                join(u, v, parent_wolf, lca_parent_wolf, children);
            }
        }
    }
    lca_parent_wolf[n - 1][0] = n - 1;

    totId = 0;
    compute_tree_ranges(n - 1, startRange_wolf, endRange_wolf, depth_wolf);

    fill(children, children + n, vector<int>());

// form the path tree for human
    init(n, parent_human);
    for (int u = n - 1; u >= 0; --u){
        for (int v : adj[u]) {
            if (v > u) {
                join(u, v, parent_human, lca_parent_human, children);
            }
        }
    }

    totId = 0;
    compute_tree_ranges(0, startRange_human, endRange_human, depth_human);


    vector<pair<int, int>> fusion;
    for(int i = 0; i < n; ++i) fusion.push_back({startRange_human[i], startRange_wolf[i]});
    sort(fusion.begin(), fusion.end());
    for(int i = 0; i < n; ++i) update(fusion[i].second, fusion[i].first, 1, 0, n);

//    for(int i = 0; i < n; ++i) {
//        lca_parent_wolf[i][0] = parent_wolf[i];
//        lca_parent_human[i][0] = parent_human[i];
//    }

    preprocess(n, lca_parent_wolf);
    preprocess(n, lca_parent_human);

    vector<q_obj> queries;
    for (int i=0;i< q;i++){
        int s,e,l,r; cin>>s>>e>>l>>r;

        int w_max = highest_ancestor_wolf(e, r, lca_parent_wolf, depth_wolf);
        int h_min = highest_ancestor_human(s, l, lca_parent_human, depth_human);

        queries.push_back({i, startRange_wolf[w_max], endRange_wolf[w_max], startRange_human[h_min], endRange_human[h_min]});
    }
    sort(queries.begin(), queries.end(), q_sort);

    vector<int> res(q);
    for(const auto& qn : queries) {
        res[qn.q] = query(qn.s, qn.e, qn.l, qn.r, 1, 0, n);
    }

    for(int i = 0; i < q; ++i) {
        cout << res[i] << "\n";
    }
}