#include <bits/stdc++.h>

using namespace std;

using ll = long long;

int main() {

    constexpr int mod = 1000000007;

    ll seq_len, queries;
    cin >> seq_len >> queries;

    vector<ll>f(seq_len + 1,-1);

    {
        ll f1, f2;
        cin >> f1 >> f2;
        f[0] = f1;
        f[1] = f2;
    }

    ll a, b;
    cin >> a >> b;

    for(int i = 2; i < seq_len + 1; ++i) {
        f[i] = (a * f[i - 2] + b * f[i - 1]) % mod;
    }

    vector<ll>x_vals;
    for(ll i = 0; i < seq_len; ++i) {
        ll x;
        cin >> x;
        x_vals.push_back(x);
    }

    vector<ll>p(seq_len + 5, 0);

    for(ll q = 0; q < queries; ++q) {
        ll l, r;
        cin >> l >> r;
        p[l] = (p[l] + f[0]) % mod;
        if(l != r) {
            p[l+1] = ((p[l+1] + f[1]) + mod - ((b * f[0]) % mod)) % mod;
            p[r + 1] = (p[r+1] + mod - f[r - l + 1]) % mod;
            p[r + 2] = (p[r+2] + mod - ((a * f[r-l]) % mod)) % mod;
        } else {
            p[r+1] = (p[r+1] + mod - ((b * f[0]) % mod)) % mod;
            p[r+2] = (p[r+2] + mod - ((a * f[0]) % mod)) % mod;
        }

    }

    for(int i = 2; i <= p.size() - 1; ++i) {
        p[i] = (p[i] + ((a * p[i - 2])%mod) + ((b * p[i - 1])%mod)) % mod;
    }

    for(ll i = 0; i < seq_len; ++i) {
        cout << (x_vals[i] + p[i + 1]) % mod << " ";
    }

    cout << "\n";
}
