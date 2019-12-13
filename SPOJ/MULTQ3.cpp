#include <bits/stdc++.h>

using namespace std;

constexpr int MAX_N = 100000;
constexpr int TREE_SIZE = 266666;

struct node {
    int subtree_vals[3];
    int lazy_counter;
};

node r_tree[TREE_SIZE];

void init_r_tree(int i, int start, int end) {

    r_tree[i].subtree_vals[0] = end - start;

    if(end - start == 1) return;
    int mid = (start + end) / 2;

    init_r_tree(2 * i, start, mid);
    init_r_tree(2 * i + 1, mid, end);
}

// Procedure for recalculating a node’s sum from its lazy and children .
void recalculate(int i, int l, int r) {
    if(r - l != 1) {
        for(int x = 0; x < 3; ++x) {
            r_tree[i].subtree_vals[x] = r_tree[2 * i].subtree_vals[x] + r_tree[2 * i + 1].subtree_vals[x];
        }
    }
}

// update lazy_counter
void update_lazy(int i, int v, int l, int r) {
    if(v == 0) return;
    r_tree[i].lazy_counter = (r_tree[i].lazy_counter + v) % 3;

    int curr = r_tree[i].subtree_vals[0];
    int curr_idx = 0;
    for(int x = 0; x < 3; ++x) {
        curr_idx = (curr_idx + v) % 3;
        int tmp = r_tree[i].subtree_vals[curr_idx];
        r_tree[i].subtree_vals[curr_idx] = curr;
        curr = tmp;
    }
}

// Preorder procedure for propagation . Do NOT call it on leaves .
void propagate(int i, int l, int r) {
    int mid = (l + r)/2;
    update_lazy(i * 2, r_tree[i].lazy_counter, l, mid);
    update_lazy(i * 2 + 1, r_tree[i].lazy_counter, mid, r);
    r_tree[i].lazy_counter = 0;
}

void update(int i, int uL, int uR, int start, int end) {
    if (uL == start && uR == end) {
        update_lazy(i, 1, start, end);
        return;
    }
    propagate(i, start, end);
    int mid = (start + end) / 2;
    if (uL < mid) update(2 * i, uL, min(uR, mid), start, mid);
    if (uR > mid) update(2 * i + 1, max(uL, mid), uR, mid, end);
    recalculate(i, start, end);
}

int query(int i, int qL, int qR, int start, int end) {
    if (qL == start && qR == end) {
        return r_tree[i].subtree_vals[0];
    }

    propagate(i, start, end);
    int mid = (start + end) / 2;
    int ans = 0;
    if (qL < mid) ans += query(2 * i, qL, min(qR, mid), start, mid);
    if (qR > mid) ans += query(2 * i + 1, max(qL, mid), qR, mid, end);
    return ans;
}

int main() {
    int n, q;
    cin >> n >> q;

    memset(r_tree, 0, sizeof(r_tree));
    init_r_tree(1, 0, n);

    for(int i = 0; i < q; ++i) {
        int q_type, a, b;
        cin >> q_type >> a >> b;
        ++b;

        if(q_type == 0) {
            update(1, a, b, 0, n);
        } else {
            cout << query(1, a, b, 0, n) << "\n";
        }
    }
}