// I think I want to convert from sparse table to range trees
// because TLE and i think the slow step is while(true) loop (expanding the implied range of a can)
// with a range tree we can update the value at the leaves of the tree as we go so the next round will be quicker

#include <bits/stdc++.h>

using namespace std;

constexpr int MAXN = 100000;
constexpr int LOGN = 18;
constexpr int INF = INT_MAX;

int x_vals[MAXN];
pair<int,int> sorted_x_vals[MAXN];

pair<int, int> ranges[MAXN];
pair<int, int> sorted_ranges[MAXN];

struct node {
    int min;
    int max;
};

node tree[5 * MAXN];

/*
 * range updates range queries 1
 */


// Procedure for recalculating a node's sum from its lazy and children.
void recalculate(int id, long long l, long long r) {
    if (r - l != 1) {
        tree[id].min = min(tree[2 * id].min, tree[2*id + 1].min);
        tree[id].max = min(tree[2 * id].max, tree[2*id + 1].max);
    }
}


/*
 * range updates range queries 2
 */

// The root node is responsible for [0, MAX_N). Update range [uL, uR)
void update(int a, int v_min, int v_max, int i, int cLeft, int cRight) {
    if (cRight - cLeft == 1) {
        tree[i].min = v_min;
        tree[i].max = v_max;
        return;
    }
    int mid = (cLeft + cRight) / 2;
    if (a < mid) update(a, v_min, v_max, i * 2, cLeft, mid);
    else update(a, v_min, v_max, i * 2 + 1, mid, cRight);
    recalculate(i, cLeft, cRight);
}

pair<int,int> query(int qL, int qR, int i, int cLeft, int cRight) {
    if (qL == cLeft && qR == cRight) {
        return {tree[i].min, tree[i].max};
    }

    int mid = (cLeft + cRight) / 2;
    pair<int,int> l = {INF, -1};
    pair<int,int> r = {INF, -1};
    if (qL < mid) l = query(qL, min(qR, mid), i * 2, cLeft, mid);
    if (qR > mid) r = query(max(qL, mid), qR, i * 2 + 1, mid, cRight);
    return {min(l.first, r.first), max(l.second, r.second)};
}

int main() {
    int n; cin >> n;

    for(int i = 0; i < n; ++i) {
        int x, r; cin >> x >> r;
        sorted_x_vals[i] = {x, i};
        ranges[i] = {x - r, x + r};
    }

    sort(sorted_x_vals, sorted_x_vals + n);

    for(int i = 0; i < n; ++i) {
        sorted_ranges[i] = ranges[sorted_x_vals[i].second];
        x_vals[i] = sorted_x_vals[i].first;
    }

    for(int i = 0; i < n; ++i) {
        sorted_ranges[i].first = (int)(lower_bound(x_vals, x_vals + n, sorted_ranges[i].first) - x_vals);
        sorted_ranges[i].second = (int)(upper_bound(x_vals, x_vals + n, sorted_ranges[i].second) - x_vals);
        --sorted_ranges[i].second;
        //ranges will be inclusive i.e. [3,3] will represent just including 3, [1,3] includes 1 to 3
    }


    pair<int, int> output[MAXN];
    for(int i = 0; i < n; ++i) {
        output[sorted_x_vals[i].second] = sorted_ranges[i];
    }

    for(int i = 0; i < n; ++i) {
        cout << output[i].second - output[i].first + 1 << " ";
    }
    cout << "\n";
}