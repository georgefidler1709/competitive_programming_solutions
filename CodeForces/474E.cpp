#include <bits/stdc++.h>

using namespace std;

using ll = long long;

vector<pair<ll,ll>> best_pred_tree;

// update an index in the "array" with a new value
// the value is of the form <some_i, max_number_of_hops_to_i>
// [start, end)
void update(ll a, pair<ll,ll> v, ll i, ll start, ll end) {

    // terminating condition is if end=start+1, i.e. start is the desired position
    if (end-start == 1) {
        // provided the max_number_of_hops we are passing in is greater
        // we update the value at this node in the best_pred_tree
        // must be a leaf node
        if (v.second > best_pred_tree[i].second) best_pred_tree[i] = v;
        return;
    }

    // mid will be floored s.t.
    // [ 1 2 3 4 5 6 7 ]
    // if start = 0 and end = 7 [start,end)
    // mid is 3
    ll mid = (start + end) / 2;

    // so is the index we are looking for in the left or right half of this range?
    if (a < mid) update(a, v, 2*i, start, mid);
    else update(a, v, 2*i+1, mid, end);

    // copy the two child nodes because we don't know how c++ memory management works
    pair<ll,ll> l_copy(best_pred_tree[2*i]);
    pair<ll,ll> r_copy(best_pred_tree[2*i+1]);

    // the value at this node is the greater of the two children
    best_pred_tree[i] = l_copy.second > r_copy.second ? l_copy : r_copy;
}

// query for the greatest number of hops in a range, and the
// [l,r)
pair<ll,ll> query(ll l, ll r, ll i, ll start, ll end) {
    if (l == start && r == end) {
        return best_pred_tree[i];
    }
    ll mid = (start + end) / 2;
    pair<ll,ll> result;
    bool set = false;
    if (l < mid) {
        result = query(l, min(mid,r), 2*i, start, mid);
        set = true;
    }
    if (r > mid) {
        pair<ll,ll> tmp = query(max(mid,l), r, 2*i+1, mid, end);
        result = (set && (result.second > tmp.second)) ? result : tmp;
    }
    return result;
}


int main() {
    ll num_pillars, min_height_diff;
    cin >> num_pillars >> min_height_diff;
    vector<ll> pillars{};

    for (ll i = 0; i < num_pillars; ++i) {
        ll p;
        cin >> p;
        pillars.push_back(p);
    }

    vector<ll> ordered_pillars = pillars;
    sort(ordered_pillars.begin(), ordered_pillars.end());

    ll tree_size = num_pillars << 1;
    tree_size = 2 << (ll)log2(tree_size);

    best_pred_tree = vector<pair<ll,ll>>(tree_size, {-1,0});

    ll last_layer_size = tree_size >> 1;

    map<ll, pair<ll,ll>> bounds{};
    map <ll, ll> height_to_idx{};
    ll l_search_limit = 0;
    ll r_search_limit = 0;

    for (ll ordered_idx = 0; ordered_idx < ordered_pillars.size(); ++ordered_idx) {
        ll curr_h = ordered_pillars[ordered_idx];

        while ((l_search_limit < ordered_pillars.size()) &&
               (ordered_pillars[l_search_limit] <= curr_h - min_height_diff))
            ++l_search_limit;
        while ((r_search_limit < ordered_pillars.size()) &&
               (ordered_pillars[r_search_limit] < curr_h + min_height_diff))
            ++r_search_limit;

        bounds[curr_h] = {l_search_limit, r_search_limit};
        height_to_idx[curr_h] = ordered_idx;
    }

    vector<ll> back;
    ll best_end = -1;
    ll best_end_idx = -1;

    for(ll pillars_idx = 0; pillars_idx < pillars.size(); pillars_idx++) {
        ll curr_h = pillars[pillars_idx];
        l_search_limit = bounds[curr_h].first;
        r_search_limit = bounds[curr_h].second;

        pair<ll,ll> l_result = {-1, -1};
        if(l_search_limit > 0) {
            l_result = max(query(0,l_search_limit, 1, 0, last_layer_size), l_result);
        }

        pair<ll,ll> r_result = {-1, -1};
        if(r_search_limit < pillars.size()) {
            r_result = max(query(r_search_limit, pillars.size(), 1, 0, last_layer_size), r_result);
        }

        ll jumps = max(l_result.second, r_result.second) + 1;
        ll pred = (l_result.second > r_result.second) ? l_result.first : r_result.first;


        back.push_back(pred);

        update(height_to_idx[curr_h], {pillars_idx, jumps}, 1, 0, last_layer_size);

        if (jumps > best_end) {
            best_end = jumps;
            best_end_idx = pillars_idx;
        }
    }

    vector<ll> seq;
    for(ll i = best_end_idx; i != -1; i = back[i]) {
        seq.push_back(i);
    }

    cout << seq.size() << "\n";

    for(auto it = seq.crbegin(); it != seq.crend(); ++it) {
        cout << *it + 1 << " ";
    }
}