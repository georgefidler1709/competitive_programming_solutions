// Sort by increasing x bc max hull trick
// bc no nested rectangles, increasing x also means decreasing y (if it had bigger x and bigger y it would consume the smaller rectangle)
//
// dp[i] = max j<i (y_i*x_i - a_i +( dp[j] -y_i*x_i ))

#include <bits/stdc++.h>

using namespace std;

using ll = long long;

#define MAXN 1000001
#define INF LLONG_MAX;

struct rec {
    ll x, y, a;

    bool operator<(const rec& a) {
        return x < a.x;
    }
};


rec arr[MAXN];

ll dp[MAXN];

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
    for(int i = 1; i <= n; ++i) {
        cin >> arr[i].x >> arr[i].y >> arr[i].a;
    }

    sort(arr + 1, arr + n + 1);

    // Base case:
    dp[0] = 0;
    line l;
    l.m = 0;
    l.b = dp[0];
    add(l);

    dp[1] = (arr[1].x * arr[1].y) - arr[1].a;
    l.m = arr[1].x;
    l.b = dp[1];
    add(l);

    for (int i = 2; i <= n; i++) {
        dp[i] = query(-arr[i].y);
        dp[i] += (arr[i].y * arr[i].x) - arr[i].a;
        l.m = arr[i].x;
        l.b = dp[i];
        add(l);
    }

    ll max = -1;
    for(int i = 0; i <= n; ++i) {
        if(dp[i] > max) max = dp[i];
    }

    cout << max << "\n";
}

