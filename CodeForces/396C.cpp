#include <bits/stdc++.h>

using namespace std;
using ll = long long;

constexpr int MAXN = 300000;
constexpr int TREE_SIZE = 1200005;
constexpr int MOD = 1000000007;

struct node {
    ll val;
    ll lazy_val;
    ll k;
    ll lazy_k;
};

node r_tree[TREE_SIZE];

void update_lazy(int i, ll x, ll k, int d) {
    r_tree[i].val = (r_tree[i].val + x + (ll)((k % MOD) * (ll)d) % MOD) % MOD;
    r_tree[i].lazy_val = (r_tree[i].lazy_val + x + (ll)((k % MOD) * (ll)d) % MOD) % MOD;
    r_tree[i].k = r_tree[i].k + k;
    r_tree[i].lazy_k = r_tree[i].lazy_k + k;

}

void propagate(int i, int start, int end) {
    if(end - start == 1) return;
    r_tree[i * 2].val = r_tree[i * 2].val + r_tree[i].lazy_val;
    r_tree[i * 2].lazy_val = r_tree[i * 2].lazy_val + r_tree[i].lazy_val;
    r_tree[i * 2].k = r_tree[i * 2].k + r_tree[i].lazy_k;
    r_tree[i * 2].lazy_k = r_tree[i * 2].lazy_k + r_tree[i].lazy_k;

    r_tree[i * 2 + 1].val = r_tree[i * 2 + 1].val + r_tree[i].lazy_val;
    r_tree[i * 2 + 1].lazy_val = r_tree[i * 2 + 1].lazy_val + r_tree[i].lazy_val;
    r_tree[i * 2 + 1].k = r_tree[i * 2 + 1].k + r_tree[i].lazy_k;
    r_tree[i * 2 + 1].lazy_k = r_tree[i * 2 + 1].lazy_k + r_tree[i].lazy_k;

    r_tree[i].lazy_val = 0;
    r_tree[i].lazy_k = 0;
}

void update(int i, int start, int end, int uL, int uR, int x, int k, int d) {
    if (uL == start && uR == end) {
        update_lazy(i, x, k, d);
        return;
    }
    propagate(i, start, end);
    int mid = (start + end) / 2;
    if (uL < mid) update(2 * i, start, mid, uL, min(uR, mid), x, k, d);
    if (uR > mid) update(2 * i + 1, mid, end, max(uL, mid), uR, x, k, d);
}

ll query(int i, int start, int end, int target, int d) {
    if (end - start == 1) {
        return (r_tree[i].val + MOD - ((ll)((r_tree[i].k % MOD) * (ll)d) % MOD)) % MOD;
    }

    propagate(i, start, end);
    int mid = (start + end) / 2;
    if (target < mid) return query(2 * i, start, mid, target, d);
    else return query(2 * i + 1, mid, end, target, d);
}

vector<int> children[MAXN+5];
// A node is responsible for the range [startRange[v], endRange[v])
int indexInRangeTree[MAXN+5], startRange[MAXN+5], endRange[MAXN+5];
int totId;
int depth[MAXN+5];

void compute_tree_ranges(int c, int d = 0) {

    indexInRangeTree[c] = startRange[c] = totId++;
    depth[c] = d;
    for (int nxt : children[c]) {
        compute_tree_ranges(nxt, d + 1);
    }
    endRange[c] = totId;
}

void update_subtree(int v, int x, int k, int n, int d) {
    update(1, 0, n, startRange[v], endRange[v], x, k, d);
}

ll query_subtree(int v, int n, int d) {
    return query(1, 0, n, startRange[v], d);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    for(int i = 1; i < n; ++i) {
        int p;
        cin >> p;
        --p;
        children[p].push_back(i);
    }

    compute_tree_ranges(0);

    int q;
    cin >> q;
    for(int qn = 0; qn < q; ++qn) {
        int q_type;
        cin >> q_type;

        switch(q_type) {
            case 1:
                int v, x, k;
                cin >> v >> x >> k;
                --v;
                update_subtree(v, x, k, n, depth[v]);
                break;
            default:
                int v2;
                cin >> v2;
                --v2;
                cout << (query_subtree(v2, n, depth[v2]) + MOD) % MOD << "\n";
        }
    }

}
