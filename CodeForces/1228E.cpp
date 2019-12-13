#include <bits/stdc++.h>

using namespace std;

using ll = long long;

#define MAX 251
#define MOD 1000000007

ll combinations[MAX][MAX];
ll pow_k[MAX];
ll pow_k_1[MAX];

ll dp[MAX][MAX];

int main() {

    for(int i = 0; i < MAX; ++i) combinations[i][0] = 1;

    for(int i = 1; i < MAX; ++i) {
        for(int j = 1; j < MAX; ++j) {
            combinations[i][j] = (combinations[i - 1][j] + combinations[i - 1][j - 1]) % MOD;
        }
    }

    int n, k; cin >> n >> k;

    pow_k[0] = pow_k_1[0] = 1;
    for(int i = 1; i < MAX; ++i) {
        pow_k[i] = (pow_k[i - 1] * k) % MOD;
        pow_k_1[i] = (pow_k_1[i - 1] * (k - 1)) % MOD;
    }

    dp[0][0] = 1;
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++) {
            for(int l = 0; l <= j; l++) {
                if (j == l) {
                    dp[i][j] = (dp[i][j] + ((dp[i - 1][l] * (MOD + pow_k[j] - pow_k_1[j]) % MOD) % MOD * pow_k_1[n - j]) % MOD) % MOD;
                } else {
                    dp[i][j] = (dp[i][j] + (((((dp[i - 1][l] * pow_k[l]) % MOD) * combinations[n - l][j - l]) % MOD) * pow_k_1[n - j] % MOD)) % MOD;
                }
            }
        }
    }

    cout << dp[n][n] << "\n";
}