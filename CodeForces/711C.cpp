# include <bits/stdc++.h>

using namespace std;

# define MAXN 101
# define INF 10000000000000
# define GREY 0

using ll = long long;

ll p_cost[MAXN][MAXN];
ll colour[MAXN];

// dp[i][j][l] = min cost of sequence [1, i], ending in colour j, s.t. beauty value of [1, i] = l
ll dp[MAXN][MAXN][MAXN];

int main() {
    for(ll i = 0; i < MAXN; ++i) {
        for(ll j = 0; j < MAXN; ++j) {
            fill(dp[i][j], dp[i][j] + MAXN, INF);
        }
    }
    ll n, m, k; cin >> n >> m >> k;

    for(ll i = 1; i <= n; ++i) {
        // initial colour for tree[i]
        cin >> colour[i];
    }

    // cost to paint tree[i] colour j
    for(ll i = 1; i <= n; ++i) {
        for(ll j = 1; j <= m; ++j) {
            cin >> p_cost[i][j];
        }

    }

    // base case
    if(colour[1] == GREY) {
        for(ll i = 1; i <= m; ++i) {
            dp[1][i][1] = p_cost[1][i];
        }
    } else {
        ll cur_c = colour[1];
        dp[1][cur_c][1] = 0;
    }

    // recurrence
    for(ll i = 2; i <= n; ++i) {
       if(colour[i] == GREY) {
           for(ll cur_c = 1; cur_c <= m; ++cur_c) {
               for(ll j = 1; j <= m; ++j) {
                   for(ll l = 1; l <= min(i - 1, k); ++l) {
                       if(j == cur_c) dp[i][cur_c][l] = min(dp[i][cur_c][l], dp[i - 1][j][l] + p_cost[i][cur_c]);
                       else dp[i][cur_c][l + 1] = min(dp[i][cur_c][l + 1], dp[i - 1][j][l] + p_cost[i][cur_c]);
                   }
               }
           }

       } else {
           ll cur_c = colour[i];
           for(ll j = 1; j <= m; ++j) {
               for(ll l = 1; l <= min(i - 1, k); ++l) {
                   if(j == cur_c) dp[i][cur_c][l] = min(dp[i][cur_c][l], dp[i - 1][j][l]);
                   else dp[i][cur_c][l + 1] = min(dp[i][cur_c][l + 1], dp[i - 1][j][l]);
               }
           }

       }
    }

    ll min_cost = INF;
    if(colour[n] == GREY) {
        for(ll j = 1; j <= m; ++j) {
            min_cost = min(min_cost, dp[n][j][k]);
        }
    } else {
        min_cost = dp[n][colour[n]][k];
    }

    if(min_cost == INF) cout << "-1" << "\n";
    else cout << min_cost << "\n";

}
