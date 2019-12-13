//observations:
// - It's always better to get a router over single room access (same price, one has the chance of covering more rooms)
// - However we may not want all routers (e.g. if all rooms have routers we only need to buy a few and spread them out)

// Think the idea is similar to the one in class (Segment Cover) where we find the best for the range [0, i] exactly. No range with and end > i is considered.
// Simplifies the recurrence

#include <bits/stdc++.h>

using namespace std;
using ll = long long;

constexpr ll MAXN = 200000;
constexpr ll INF = LLONG_MAX;

using seg = tuple<ll, ll, ll>; //start, end, value

// no idea if this is actually works in all cases but hey it passed the autotests
// sort by end point (so we can easily work on the segment [0, end])
// if they have the same end point, prioritise the bigger seg
// if they're same size prioritise cheaper seg
struct seg_cmp {
    bool operator()(const seg& a, const seg& b) {
        ll s_a, e_a, v_a;
        tie(s_a, e_a, v_a) = a;
        ll s_b, e_b, v_b;
        tie(s_b, e_b, v_b) = b;

        if(e_a != e_b) return e_a < e_b;
        if(s_a != s_b) return s_a < s_b;
        else return v_a < v_b;
    }

};


// range tree is just a min tree
// range [a, b) stores the minimum cost to cover the range [0, a] using only segs ending before b
ll dp[5 * MAXN];
set<seg, seg_cmp> segs;

/*
 * range updates range queries 1
 */

// Procedure for recalculating a node's sum from its lazy and children.
void recalculate(ll id, long long l, long long r) {
    if (r - l != 1) {
        dp[id] = min(dp[2 * id], dp[2 * id + 1]);
    }
}

/*
 * range updates range queries 2
 */

// The root node is responsible for [0, MAX_N). Update range [uL, uR)
void update(ll a, ll v, ll i, ll cLeft, ll cRight) {
    if (cRight - cLeft == 1) {
        dp[i] = min(dp[i], v);
        return;
    }

    ll mid = (cLeft + cRight) / 2;
    if (a < mid) update(a, v, i * 2, cLeft, mid);
    else update(a, v, i * 2 + 1, mid, cRight);
    recalculate(i, cLeft, cRight);
}

ll query(ll qL, ll qR, ll i, ll cLeft, ll cRight) {
    if (qL == cLeft && qR == cRight) {
        return dp[i];
    }

    ll mid = (cLeft + cRight) / 2;
    ll m = INF;
    if (qL < mid) m = min(m, query(qL, min(qR, mid), i * 2, cLeft, mid));
    if (qR > mid) m = min(m, query(max(qL, mid), qR, i * 2 + 1, mid, cRight));
    return m;
}


int main() {
    fill(dp, dp + 5 * MAXN, INF);
    ll n, k; cin >> n >> k;

    for(ll i = 1; i <= n; ++i) {
        segs.insert(seg(i, i, i));
        char c; cin >> c;
        if(c == '1') {
            // segs can't go over the endpoints
            ll seg_start = max((ll)1, i - k);
            ll seg_end = min(i + k, n);
            segs.insert(seg(seg_start, seg_end, i));
        }
    }

    // Base case: for range [1,1] we can never do better than 1.
    update(0, 1, 1, 0, n);

    for(ll i = 2; i <= n; ++i) {
        // get best seg for endpoint i
        auto s = *segs.upper_bound(seg(0, i, 0));
        ll start, end, cost;
        std::tie(start, end, cost) = s;

        // find cost for covering the area preceeding the start of the seg using the range tree
        ll i_cost = (start - 2 < 0) ? cost : cost + query(start - 2, end, 1, 0, n);

        update(i, i_cost, 1, 1, n + 1);
    }

    std::cout << query(n - 1, n, 1, 0 , n) << "\n";



}