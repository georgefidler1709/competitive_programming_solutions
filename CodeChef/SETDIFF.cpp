#include <bits/stdc++.h>

using namespace std;

using ll = long long;

#define MOD 1000000007
#define MAXN 100001

int a[MAXN];
ll powers[MAXN];

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    powers[0] = 1;
    for (int i = 1; i < MAXN; i++) {
        powers[i] = (powers[i - 1] << 1) % MOD;
    }

    int cases; cin >> cases;

    for(int c = 0; c < cases; ++c) {
        int n; cin >> n;
        for(int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        sort(a, a + n);

        ll before = 0;
        ll after = 0;

        for(int i = 0; i < n; ++i) {
            before = (before + a[i] * powers[i]) % MOD;
            after = (after + a[i] * powers[n - i - 1]) % MOD;
        }

        cout << ((MOD + before - after) % MOD) << "\n";
    }

}