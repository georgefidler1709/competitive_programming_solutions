#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

using namespace std;

typedef tree
        <int, null_type, less_equal<int>, rb_tree_tag,
                tree_order_statistics_node_update>
        ordered_set;

constexpr int MAX_N = 200000;

struct node {
    ordered_set fruit;
};

node r_tree[4 * MAX_N + 5];

void recalculate(int i, int v) {
    r_tree[i].fruit.insert(v);
}

void update(int i, int start, int end, int target, int v) {
    if(end - start == 1) {
        r_tree[i].fruit.insert(v);
        return;
    }

    int mid = (start + end) / 2;
    if(target < mid) update(i * 2, start, mid, target, v);
    else update(i * 2 + 1, mid, end, target, v);

    recalculate(i, v);
}

int query(int i, int start, int end, int qL, int qR, int v) {
    if (qL == start && qR == end) {
        return r_tree[i].fruit.order_of_key(v);
    }

    int ans = 0;
    int mid = (start + end) / 2;
    if (qL < mid) ans += query(2 * i, start, mid, qL, min(qR, mid), v);
    if (qR > mid) ans += query(2 * i + 1, mid, end, max(qL, mid), qR, v);
    return ans;
}

vector<int> children[MAX_N+5];
// A node is responsible for the range [startRange[v], endRange[v])
int indexInRangeTree[MAX_N+5], startRange[MAX_N+5], endRange[MAX_N+5];
int totId;

void compute_tree_ranges(int c, int prev = -1) {

    indexInRangeTree[c] = startRange[c] = totId++;
    for (int nxt : children[c]) {
        if(nxt != prev) {
            compute_tree_ranges(nxt, c);
        }
    }
    endRange[c] = totId;
}

void update_node(int id, int v, int n) {
    update(1, 0, n, indexInRangeTree[id], v);
}

int query_subtree(int id, int v, int n) {
    return query(1, 0, n, startRange[id], endRange[id], v);
}


int main() {
    int n, q;
    cin >> n >> q;

    int tree_to_first_fruit[MAX_N] = {0};
    for(int i = 0; i < n; ++i) {
        cin >> tree_to_first_fruit[i];
    }

    for(int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        children[u].push_back(v);
        children[v].push_back(u);
    }
    memset(indexInRangeTree, -1, sizeof indexInRangeTree);
    compute_tree_ranges(0);

    for(int i = 0; i < n; ++i) {
        update_node(i, tree_to_first_fruit[i], n);
    }

    for(int q_n = 0; q_n < q; ++q_n) {
        int a, s;
        cin >> a >> s;
        update_node(a, s, n);
        cout << query_subtree(a, s, n) << "\n";
    }
}