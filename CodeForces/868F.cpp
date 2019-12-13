#include <bits/stdc++.h>

using namespace std;

#define MAXN 100001
#define MAXK 21
#define INF 1e17;

using ll = long long;

int num_per_type[MAXN];
ll dp[MAXK][MAXN];
int a[MAXN];

int l = 0, r = 0;
ll total = 0;


// Preloading all MAXN * MAXN totals for i and j would be O(n^2) which defeats the point of the divide and conquer speed up
// But because we know we are divide and conquering we know for each of the k passes we will look left in (roughly) halving segments
// And then right in halving segments
// So the average amount of moves r and l need to make each time this is called is around log(n)
// Or at least over the entire pass it won't move more than 2n
// so for k passes thats at most nk instead of n^2 and k is way less than n

// Cost of segment (i, j]
ll Cost(int i, int j) {
    while(r < j) {
        ++r;
        total += num_per_type[a[r]];
        ++num_per_type[a[r]];
    }

    while(r > j) {
        --num_per_type[a[r]];
        total -= num_per_type[a[r]];
        --r;
    }

    while(l < i) {
        ++l;
        --num_per_type[a[l]];
        total -= num_per_type[a[l]];
    }

    while(l > i) {
        total += num_per_type[a[l]];
        ++num_per_type[a[l]];
        --l;
    }

    return total;
}

void dnc(int qL, int qR, int cL, int cR, int k) {
    if (cR <= cL) return;
    int bestpos;
    int mid = (cL + cR) / 2;
    dp[k][mid] = INF;
    for (int i = qL; i <= min(qR, mid-1); i++) {
        // Cost expects (], so we use Cost(i, mid).
        long long newcost = dp[k-1][i] + Cost(i, mid);
        if (newcost < dp[k][mid]) {
            dp[k][mid] = newcost;
            bestpos = i;
        }
    }
    dnc(qL, bestpos, cL, mid, k);
    dnc(bestpos, qR, mid+1, cR, k);
}



int main() {
    int n, k; cin >> n >> k;

    for (int i = 1; i <= n; i++) {
        int c; scanf("%d", &c);
        a[i] = c;
    }

    // For K = 0, dp[0][0] = 0 is the base case.
    for (int i = 1; i <= n; i++) dp[0][i] = INF;
    // Just call dnc for k from 1 to N
    // Make sure you get the initial [qL,qR] and [cL, cR) correct.
    for (int i = 1; i <= k; i++) {
        dnc(0, n, 1, n+1, i);
    }
    printf("%lld\n", dp[k][n]);
    return 0;
}