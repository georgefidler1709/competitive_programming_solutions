#include <bits/stdc++.h>

using namespace std;

#define MAXN 1000001

int arr[MAXN];
vector<int> children[MAXN + 1];

int indexInRangeTree[MAXN+5], startRange[MAXN+5], endRange[MAXN+5];
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

long long lazy[5 * MAXN];
long long max_seq[5 * MAXN];

// Procedure for recalculating a node's sum from its lazy and children.
void recalculate(int id, long long l, long long r) {
    if (r - l != 1) {
        max_seq[id] = max(max_seq[2 * id], max_seq[2 * id + 1]);
    }
}

void update_lazy(int id, long long v, long long l, long long r) {
    lazy[id] += v;
    max_seq[id] += v;
}

// Preorder procedure for propagation. Do NOT call it on leaves.
void propagate(int id, long long l, long long r) {
    long long mid = (l + r) / 2;
    update_lazy(id * 2, lazy[id], l, mid);
    update_lazy(id * 2 + 1, lazy[id], mid, r);
    lazy[id] = 0;
}

/*
 * range updates range queries 2
 */

// The root node is responsible for [0, MAX_N). Update range [uL, uR)
void update(int uL, int uR, int v, int i, int cLeft, int cRight) {
    if (uL == cLeft && uR == cRight) {
        update_lazy(i, v, cLeft, cRight);
        return;
    }
    propagate(i, cLeft, cRight);
    int mid = (cLeft + cRight) / 2;
    if (uL < mid) update(uL, min(uR, mid), v, i * 2, cLeft, mid);
    if (uR > mid) update(max(uL, mid), uR, v, i * 2 + 1, mid, cRight);
    recalculate(i, cLeft, cRight);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k; cin >> n >> k;
    for(int i = 0; i < n; ++i) {
        cin >> arr[i];
    }

    stack<int> s;
    for(int i = n - 1; i >= 0; --i) {
        while(!s.empty() && arr[s.top()] <= arr[i]) s.pop();
        if(!s.empty()){
            children[s.top()].push_back(i);
        }else {
            children[n].push_back(i);
        }
        s.push(i);
    }

   compute_tree_ranges(n);

    for(int i = 0; i < n; ++i) {
        if(i < k) {
            update(startRange[i], endRange[i], 1, 1, 1, n + 1);
        } else {
            cout << max_seq[1] << " ";
            update(startRange[i - k], endRange[i - k], -1, 1, 1, n + 1);
            update(startRange[i], endRange[i], 1, 1, 1, n + 1);
        }
    }
    cout << max_seq[1] << "\n";
}