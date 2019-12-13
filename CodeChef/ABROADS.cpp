#include <bits/stdc++.h>

using namespace std;

using ll = long long;

constexpr int n = 500001;

vector<ll>parent(n);
vector<ll>node_to_pop(n);
vector<ll>subtree_size(n);


void init(ll n) {
    for (ll i = 1; i <= n; i++) {
        parent[i] = i;
    }
}

ll root(ll x) {
    // only roots are their own parents
    // otherwise apply path compression
    return parent[x] == x ? x : parent[x] = root(parent[x]);
}

void join(ll x, ll y) {
    // size heuristic
    // hang smaller subtree under root of larger subtree
    x = root(x); y = root(y);
    if (x == y) return;
    if (subtree_size[x] < subtree_size[y]) {
        parent[x] = y;
        subtree_size[y] += subtree_size[x];
    } else {
        parent[y] = x;
        subtree_size[x] += subtree_size[y];
    }
}


int main() {
    ll n_cities, n_roads, n_queries;
    cin >> n_cities >> n_roads >> n_queries;

    for (ll i = 1; i <= n_cities; ++i) {
        ll pop;
        cin >> pop;
        node_to_pop[i] = pop;
        subtree_size[i] = pop;
    }

    init(n_cities);

    set<pair<ll, ll>> curr_edges;
    vector<pair<ll, ll>> edges_in_order{{-1, -1}};

    for (ll i = 0; i < n_roads; ++i) {
        ll u, v;
        cin >> u >> v;
        pair<ll, ll> e{u, v};
        curr_edges.insert(e);
        edges_in_order.emplace_back(e);
    }

    vector<pair<char, vector<ll>>> queries{};

    for (ll i = 0; i < n_queries; ++i) {
        char q_type;
        ll num;

        cin >> q_type >> num;

        if (q_type == 'D') {
            vector<ll> nums = {num};
            pair<char, vector<ll>> q{q_type, nums};
            queries.push_back(q);
            auto edge = edges_in_order[num];
            curr_edges.erase(edge);

        } else {
            ll pop;
            cin >> pop;
            vector<ll> nums = {num, node_to_pop[num]};
            pair<char, vector<ll>> q{q_type, nums};
            queries.push_back(q);
            subtree_size[root(num)] += pop - node_to_pop[num];
            node_to_pop[num] = pop;
        }
    }

    for(const auto& e : curr_edges)
        join(e.first, e.second);

    set<pair<ll, ll>, greater<pair<ll, ll>>> ordered_pops;
    for (ll i = 1; i <= n_cities; ++i)
        ordered_pops.insert(make_pair(subtree_size[root(i)], root(i)));

    vector<ll> output;
    for (auto q = queries.crbegin(); q != queries.crend(); ++q) {
        output.emplace_back(ordered_pops.begin()->first);
        if(q->first == 'D') {
            auto e = edges_in_order[q->second[0]];
            ordered_pops.erase(make_pair(subtree_size[root(e.first)], root(e.first)));
            ordered_pops.erase(make_pair(subtree_size[root(e.second)], root(e.second)));
            join(e.first, e.second);
            ordered_pops.insert(make_pair(subtree_size[root(e.first)], root(e.first)));
        } else {
            ll curr_city = q->second[0];
            ll pop = q->second[1];
            ll prev_pop = node_to_pop[curr_city];
            node_to_pop[curr_city] = pop;
            ordered_pops.erase(make_pair(subtree_size[root(curr_city)], root(curr_city)));
            subtree_size[root(curr_city)] += pop - prev_pop;
            ordered_pops.insert(make_pair(subtree_size[root(curr_city)], root(curr_city)));
        }

    }

    for(auto it = output.crbegin(); it != output.crend(); ++it) {
        cout << *it << "\n";
    }

}