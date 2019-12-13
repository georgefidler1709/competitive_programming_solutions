#include <bits/stdc++.h>

using namespace std;

using ll = long long;

#define MAXN 1000005
#define INF LLONG_MAX;

struct rec {
    ll x, y;

    bool operator<(const rec& a) {
        return x < a.x;
    }
};


ll dp[MAXN];

bool max_rec_sort(const rec& a, const rec& b) {
    if(a.x == b.x) return a.y > b.y;
    return a.x > b.x;
}

/*
 * cht full
 */

struct line { long long m, b; };
double intersect(line a, line b) {
    return (double)(b.b - a.b) / (a.m - b.m);
}
// Invariant: cht[i].m is in decreasing order.
vector<line> cht;

void add(line l) {
    auto n = cht.size();
    while (n >= 2 &&
           intersect(cht[n-1], cht[n-2]) >= intersect(cht[n-1], l)) {
        cht.pop_back();
        n = cht.size();
    }
    cht.push_back(l);
}

long long query(long long x) {
    int lo = 0; int hi = cht.size()-2;
    // Find largest idx such that x <= intersect(cht[idx], cht[idx+1])
    int idx = cht.size()-1;
    while (lo <= hi) {
        int mid = (lo+hi)/2;
        if (intersect(cht[mid], cht[mid+1]) >= x) {
            idx = mid; hi = mid-1;
        } else { lo = mid+1; }
    }
    return cht[idx].m*x + cht[idx].b;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n; cin >> n;

    vector<rec> recs;
    for(int i = 1; i <= n; ++i) {
        int x, y;
        cin >> x >> y;
        recs.push_back({x,y});
    }

    sort(recs.begin(), recs.end(), max_rec_sort);

    // only need to keep track of max y because x is sorted in descending order
    ll max_y = -1;
    for(auto r = recs.begin(); r != recs.end(); ++r) {
        while(r != recs.end() && r->y <= max_y) {
            r = recs.erase(r);
        }
        if(r == recs.end()) break;

        if(r->y > max_y) max_y = r->y;
    }

    sort(recs.begin(), recs.end());

    // Base case:
    dp[0] = 0;
    line l;
    l.m = recs[0].y;
    l.b = dp[0];
    add(l);

    for (int i = 1; i <= recs.size(); i++) {
        dp[i] = query(recs[i - 1].x);

        if(i == recs.size()) break;

        l.m = recs[i].y;
        l.b = dp[i];
        add(l);
    }

    cout << dp[recs.size()] << "\n";
}

