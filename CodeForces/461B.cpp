#include <bits/stdc++.h>

using namespace std;
using ll = long long;

#define MAXN 100000
#define MOD 1000000007

bool colour[MAXN];

vector<ll>children[MAXN];


// trees rooted at i which do contain a black
ll black_trees[MAXN];
// trees rooted at i which do not contain a black
ll white_trees[MAXN];

void makeTrees(ll cur) {
    // base case:
    // a single node will have 1 black tree if it is black
    // 0 otherwise
    // and vice versa for white
    black_trees[cur] = colour[cur];
    white_trees[cur] = !colour[cur];

    for(auto c : children[cur]) {
        // need subtrees finished first
        makeTrees(c);

        black_trees[cur] = (
                ((black_trees[cur] * black_trees[c]) % MOD) +
                ((black_trees[cur] * white_trees[c]) % MOD) +
                ((white_trees[cur] * black_trees[c]) % MOD)
                ) % MOD;

        white_trees[cur] = (
                ((white_trees[cur] * white_trees[c]) % MOD) +
                ((white_trees[cur] * black_trees[c]) % MOD)
                ) % MOD;
    }
}

int main() {
    ll n; cin >> n;

    for(ll i = 1; i < n; ++i) {
        ll p; cin >> p;
        children[p].push_back(i);
    }

    for(ll i = 0; i < n; ++i) {
        cin >> colour[i];
    }

    makeTrees(0);

    cout << black_trees[0] << "\n";
}