#include <bits/stdc++.h>

using namespace std;

using ll = long long;

#define MAX 1001
#define MOD 1000000007

int num_per_colour[MAX];

ll combinations[MAX][MAX];

ll dp(int total, int colour) {
    if(colour == 1) return 1;
    return (combinations[total - 1][num_per_colour[colour] - 1] * dp(total - num_per_colour[colour], colour - 1)) % MOD;
}

int main() {
    int k; cin >> k;

    int total = 0;
    for(int i = 1; i <= k; ++i) {
        cin >> num_per_colour[i];
        total += num_per_colour[i];
    }

    for(int i = 0; i < MAX; ++i) combinations[i][0] = 1;

    for(int i = 1; i < MAX; ++i) {
        for(int j = 1; j < MAX; ++j) {
            combinations[i][j] = (combinations[i - 1][j] + combinations[i - 1][j - 1]) % MOD;
        }
    }

    cout << dp(total, k) << "\n";
}