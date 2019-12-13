#include <bits/stdc++.h>

using namespace std;

using ll = long long;
int main() {

    ll n_acts, n_rooms;
    cin >> n_acts >> n_rooms;

    vector<pair<ll,ll>>acts(n_acts);

    for(ll i = 0; i < n_acts; ++i) {
        ll start, end;
        cin >> start >> end;
        acts[i] = {end, start};
    }

    sort(acts.begin(), acts.end());

    multiset<ll>cur_acts;

    ll max_acts = 0;
    for(auto act : acts) {
        auto best_room = cur_acts.upper_bound(-act.second);
        if(best_room != cur_acts.end()) {
            cur_acts.erase(best_room);
            cur_acts.insert(-act.first);
            ++max_acts;
        } else if(cur_acts.size() < n_rooms) {
            cur_acts.insert(-act.first);
            ++max_acts;
            continue;
        }
    }

    cout << max_acts << "\n";
}

