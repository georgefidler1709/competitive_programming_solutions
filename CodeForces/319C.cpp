// dp[i] = cost to cut down trees 1 to i
// dp[i] = min(dp[j] + b[j] * a[i]) for j from 1 to i - 1

#include <bits/stdc++.h>

using namespace std;

using ll = long long;

#define MAXN 100000
#define INF LLONG_MAX;

int a[MAXN];
int b[MAXN];

ll dp[MAXN];

/*
 * cht increasing queries
 */

struct line { long long m, b; };
double intersect(line a, line b) {
    return (double)(b.b - a.b) / (a.m - b.m);
}
// Invariant: cht[i].m is in decreasing order.
vector<line> cht;
int cp;

void add(line l) {
    auto n = cht.size();
    while (n >= 2 &&
           intersect(cht[n-1], cht[n-2]) >= intersect(cht[n-1], l)) {
        cht.pop_back();
        n = cht.size();
    }
    cht.push_back(l);
    cp = min(cp, (int)cht.size()-1);
}

long long query(long long x) {
    while (cp+1 != cht.size() &&
           intersect(cht[cp], cht[cp+1]) < x) cp++;
    return cht[cp].m*x + cht[cp].b;
}


int main() {
    int n; cin >> n;
    for(int i = 0; i < n; ++i) cin >> a[i];
    for(int i = 0; i < n; ++i) cin >> b[i];

    // Base case:
    dp[0] = 0;
    line l;
    l.m = b[0];
    l.b = dp[0];
    add(l);

    for (int i = 1; i < n; i++) {
        dp[i] = query(a[i]);
        line l;
        l.m = b[i];
        l.b = dp[i];
        add(l);
    }

    cout << dp[n - 1] << "\n";

}
