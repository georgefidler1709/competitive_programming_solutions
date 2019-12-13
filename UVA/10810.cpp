#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;

using ll = long long;

typedef tree
        <ll, null_type, less<ll>, rb_tree_tag,
                tree_order_statistics_node_update>
        ordered_set;

ll countSwaps(const vector<ll>& v) {
    ll swaps = 0;
    ordered_set ordered_preds{};
    for(int i = 0; i < v.size(); ++i) {
        ordered_preds.insert(v[i]);
        swaps += i - ordered_preds.order_of_key(v[i]);
    }
    return swaps;
}


int main() {
    int seq_size;

    while(cin >> seq_size) {
        if(seq_size == 0) break;
        vector<ll> v{};
        for(int i = 0; i < seq_size; ++i) {
            ll next_int;
            cin >> next_int;
            v.push_back(next_int);
        }
        cout << countSwaps(v) << "\n";
    }
}