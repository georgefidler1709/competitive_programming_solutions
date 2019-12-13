#include <bits/stdc++.h>

using namespace std;

void init_rtree(vector<int>& r_tree, int depth, int root_idx, int tree_height) {
    if(r_tree[root_idx] != -1) return;

    if(r_tree[2 * root_idx] == -1) init_rtree(r_tree, depth + 1, 2 * root_idx, tree_height);
    if(r_tree[2 * root_idx + 1] == -1) init_rtree(r_tree, depth + 1, 2 * root_idx + 1, tree_height);

    if(tree_height % 2 == depth % 2) {
        r_tree[root_idx] = r_tree[2 * root_idx] ^ r_tree[2 * root_idx + 1];
    } else {
        r_tree[root_idx] = r_tree[2 * root_idx] | r_tree[2 * root_idx + 1];
    }
}

int main() {
    int n, queries;
    cin >> n >> queries;
    int nums = 1 << n;
    int tree_size = (nums << 1);
    int tree_height = n + 1;

    vector<int>r_tree(tree_size, -1);


    for(int i = tree_size - nums; i < tree_size; ++i) {
        int next_int;
        cin >> next_int;
        r_tree[i] = next_int;
    }

    init_rtree(r_tree, 1, 1, tree_height);

    for(int i = 0; i < queries; ++i) {
        int idx, val;
        cin >> idx >> val;
        idx = tree_size - nums + idx - 1;
        r_tree[idx] = val;
        int parent_idx = idx / 2;
        bool or_op = true;
        while(parent_idx > 0) {
            if(or_op) {
                r_tree[parent_idx] = r_tree[2 * parent_idx] | r_tree[2 * parent_idx + 1];
                or_op = false;
            } else {
                r_tree[parent_idx] = r_tree[2 * parent_idx] ^ r_tree[2 * parent_idx + 1];
                or_op = true;
            }
            parent_idx /= 2;
        }

        cout << r_tree[1] << "\n";
    }
}