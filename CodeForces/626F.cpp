#include <bits/stdc++.h>

using namespace std;

using ll = long long;

#define MAXN 203
#define MAXK 1001
#define MOD 1000000007

int a[MAXN];

// dp[i][j][m]
// i = curr element from left to right
// j = number of open groups
// m = current total group imbalance
int dp[MAXN][MAXN][MAXK];

int main() {
    int n, k; cin >> n >> k;

    for(int i = 1; i <= n; ++i) cin >> a[i];

    sort(a + 1, a + n + 1);

    dp[1][0][0] = 1;
    dp[1][1][0] = 1;

    for(int i = 1; i <= n; ++i) {
        for(int j = 0; j <= n; ++j) {
            for(int m = 0; (m + (a[i + 1] - a[i]) * j) <= k; ++m) {
                int pair_val = m + (a[i + 1] - a[i]) * j;
                int cur = dp[i][j][m];

                // 1) add cur to a new group and close it
                dp[i + 1][j][pair_val] = ((ll)dp[i + 1][j][pair_val] + cur) % MOD;

                // add cur to a new group and leave it open
                dp[i + 1][j + 1][pair_val] = ((ll)dp[i + 1][j + 1][pair_val] + cur) % MOD;

                // add cur to a pre-existing group and close it
                if(j > 0) dp[i + 1][j - 1][pair_val] =
                                  ((ll)dp[i + 1][j - 1][pair_val] + ((j * (ll)cur) % MOD)) % MOD;

                // add cur to a pre-existing group and leave it open
                dp[i + 1][j][pair_val] =
                        ((ll)dp[i + 1][j][pair_val] + ((j * (ll)cur) % MOD)) % MOD;
            }
        }
    }

    // count sets of groups that kept under the k threshold
    // and that closed all groups eventually
    int ans = 0;
    for(int i = 0; i <= k; ++i) {
        ans = (ans + dp[n][0][i]) % MOD;
    }
    cout << ans << "\n";

}