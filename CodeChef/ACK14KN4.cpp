#include <bits/stdc++.h>

using namespace std;

using ll = long long;

#define MAX_N 100000

struct node {
    ll sum;
    ll max;
    ll min;
    bool inc;
    bool dec;

    node() : sum{0}, max{0}, min{0}, inc{true}, dec{false} {};

};

node tree[MAX_N * 3];

// Procedure for recalculating a node's sum from its lazy and children.
void recalculate(ll id, ll l, ll r) {
    if (r - l != 1) {
        tree[id].sum = tree[2 * id].sum + tree[2 * id + 1].sum;
        tree[id].max = max(tree[2 * id].max, tree[2 * id + 1].max);
        tree[id].min = min(tree[2 * id].min, tree[2 * id + 1].min);
        tree[id].inc = (tree[2 * id].inc && tree[2 * id + 1].inc && tree[2 * id].max <= tree[2 * id + 1].min);
        tree[id].dec = (tree[2 * id].dec && tree[2 * id + 1].dec && tree[2 * id].min >= tree[2 * id + 1].max);
    }
}

/*
 * range updates range queries 2
 */

// The root node is responsible for [0, MAX_N). Update range [uL, uR)
void update(ll a, ll v, ll i, ll cLeft, ll cRight) {
    if (cRight - cLeft == 1) {
        tree[i].sum = v;
        tree[i].max = v;
        tree[i].min = v;
        tree[i].inc = true;
        tree[i].dec = true;
        return;
    }

    ll mid = (cLeft + cRight) / 2;
    if (a < mid) update(a, v, i * 2, cLeft, mid);
    else update(a, v, i * 2 + 1, mid, cRight);
    recalculate(i, cLeft, cRight);
}

ll query_sum(ll qL, ll qR, ll i, ll cLeft, ll cRight) {
    if (qL == cLeft && qR == cRight) {
        return tree[i].sum;
    }

    ll mid = (cLeft + cRight) / 2;
    long long ans = 0;
    if (qL < mid) ans += query_sum(qL, min(qR, mid), i * 2, cLeft, mid);
    if (qR > mid) ans += query_sum(max(qL, mid), qR, i * 2 + 1, mid, cRight);
    return ans;
}

ll query_max(ll qL, ll qR, ll i, ll cLeft, ll cRight) {
    if (qL == cLeft && qR == cRight) {
        return tree[i].max;
    }

    ll mid = (cLeft + cRight) / 2;
    long long m = -1;
    if (qL < mid) m = max(m, query_max(qL, min(qR, mid), i * 2, cLeft, mid));
    if (qR > mid) m = max(m, query_max(max(qL, mid), qR, i * 2 + 1, mid, cRight));
    return m;
}

node mergeStates(node l, node r) {
    node m;
    m.sum = l.sum + r.sum;
    m.max = max(l.max, r.max);
    m.min = min(l.min, r.min);
    m.inc = (l.inc && r.inc && l.max <= r.min);
    m.dec = (l.dec && r.dec && l.min >= r.max);

    return m;
}


node query_inc(ll qL, ll qR, ll i, ll cLeft, ll cRight) {
    if (qL == cLeft && qR == cRight) {
        return tree[i];
    }

    ll mid = (cLeft + cRight) / 2;

    if (qR <= mid) return query_inc(qL , qR , i * 2, cLeft , mid );
    if (qL >= mid) return query_inc(qL , qR , i * 2 + 1, mid , cRight);
    return mergeStates(
            query_inc(qL, min(qR ,mid), i * 2, cLeft, mid),
            query_inc(max(qL, mid), qR, i * 2 + 1, mid, cRight)
    );

}

node query_dec(ll qL, ll qR, ll i, ll cLeft, ll cRight) {
    if (qL == cLeft && qR == cRight) {
        return tree[i];
    }

    ll mid = (cLeft + cRight) / 2;

    if (qR <= mid) return query_dec(qL , qR , i * 2, cLeft , mid );
    if (qL >= mid) return query_dec(qL , qR , i * 2 + 1, mid , cRight);
    return mergeStates(
            query_dec(qL, min(qR ,mid), i * 2, cLeft, mid) ,
            query_dec(max(qL, mid), qR, i * 2 + 1, mid, cRight)
    );
}

int main() {

    ll n, m;
    cin >> n >> m;

    for(ll i = 0; i < n; ++i) {
        ll val; cin >> val;
        update(i, val, 1, 0, n);
    }

    for(ll i = 0; i < m; ++i) {
        char q_type;
        cin >> q_type;
        ll x, y;
        cin >> x >> y;
        x--;
        if(q_type == 'U') {
            update(x, y, 1, 0, n);
        } else if(q_type == 'M') {
            cout << query_max(x, y, 1, 0, n) << "\n";
        } else if(q_type == 'S') {
            cout << query_sum(x, y, 1, 0, n) << "\n";
        } else if(q_type == 'I') {
            cout << query_inc(x, y, 1, 0, n).inc << "\n";
        } else if(q_type == 'D') {
            cout << query_dec(x, y, 1, 0, n).dec << "\n";
        }
    }


}