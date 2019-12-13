// Each range is a node, 0 length edge between overlapping ranges (BUT CAN'T BE THIS EXACTLY e.g. 6 -> [2,7] -> [2,4] using only 0 length edges!!!). Could be a workaround to make this strat work
//

#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct edge {
    int dest;
    ll w;
};
bool operator<(const edge& a, const edge& b) {
    return a.w < b.w;
}
bool operator>(const edge& a, const edge& b) {
    return !(a < b);
}

vector<vector<edge>> plans;
vector<ll> cost_per_node;

void init_one_to_many_tree(int offset, int start, int end, int i) {
    // connects [v,v] range (i.e. bottom nodes of the tree)
    // to single planet nodes (for plans of type 1)
    if(start == end) {
        plans[i + offset].push_back({start, 0});
        return;
    }

    int mid = (start + end) / 2;

    init_one_to_many_tree(offset, start, mid, 2 * i);
    init_one_to_many_tree(offset, mid + 1, end, (2 * i) + 1);

    // make weight 0 connections down the tree
    // i.e. if v can reach [1,4] it can reach [1,2], [3,4], 1, 2, 3 and/or 4.
    plans[i + offset].push_back({(2 * i) + offset, 0});
    plans[i + offset].push_back({(2 * i) + 1 + offset, 0});
}

void init_many_to_one_tree(int offset, int start, int end, int i) {

    if (start == end) { //
        plans[start].push_back({i + offset, 0});
        return;
    }

    int mid = (start + end) / 2;

    init_many_to_one_tree(offset, start, mid, 2 * i);
    init_many_to_one_tree(offset, mid + 1, end, (2 * i) + 1);


    // make weight 0 connections up the tree
    // i.e. if v can be reached from [2,3] it can also be reached from [1,4] and/or [1,8]
    plans[(2 * i) + offset].push_back({i + offset, 0});
    plans[(2 * i) + 1 + offset].push_back({i + offset, 0});
}

void update_one_to_many_tree(int offset, int start, int end, int i, int l, int r, int src, int w) {
    if (start > r || end < l) return;

    if(start >= l && end <= r) { // within the range s.t. this range is reachable from src
        plans[src].push_back({i + offset, w});
    } else {
        int mid = (start + end) / 2;
        update_one_to_many_tree(offset, start, mid, 2 * i, l, r, src, w);
        update_one_to_many_tree(offset, mid + 1, end, (2 * i) + 1, l, r, src, w);

    }

}

void update_many_to_one_tree(int offset, int start, int end, int i, int l, int r, int dst, int w) {

    if (start > r || end < l) return;


    if(start >= l && end <= r) { // range contains the range [l,r] therefore can also reach dst from this range
        plans[i + offset].push_back({dst, w});
    } else {
        int mid = (start + end) / 2;
        update_many_to_one_tree(offset, start, mid, 2 * i, l, r, dst, w);
        update_many_to_one_tree(offset, mid + 1, end, (2 * i) + 1, l, r, dst, w);
    }
}


void dijkstra(int src, ll n_nodes, vector<vector<edge>>& plans, vector<ll>& cost_per_node) {
    vector<bool>visited(n_nodes, false);
    priority_queue<edge, vector<edge>, greater<edge>> pq;
    pq.push({src, 0});

    while(!pq.empty()) {
        auto curr = pq.top();
        pq.pop();
        if(visited[curr.dest]) continue;
        visited[curr.dest] = true;
        for(auto& e : plans[curr.dest]) {
            if(!visited[e.dest] && cost_per_node[curr.dest] + e.w < cost_per_node[e.dest]) {
                cost_per_node[e.dest] = cost_per_node[curr.dest] + e.w;
                pq.push({e.dest, cost_per_node[e.dest]});
            }
        }
    }
}

int main() {
    int n_planets, n_plans, e_idx;
    cin >> n_planets >> n_plans >> e_idx;
    --e_idx;


    int tree_size = n_planets << 1;
    tree_size = 2 << (int)log2(tree_size);

    // vector stores all nodes i.e. all ranges
    // first part is type 1 edges (u to v)
    // second part is type 2 edges (v to [l,r])
    // third part is type 3 edges ([l,r] to v)
    ll n_nodes = n_planets + 2 * tree_size;
    plans = vector<vector<edge>>(n_nodes, vector<edge>());
    int one_to_many_offset = n_planets;
    int many_to_one_offset = n_planets + tree_size;

    init_one_to_many_tree(one_to_many_offset, 0, n_planets - 1, 1);
    init_many_to_one_tree(many_to_one_offset, 0, n_planets - 1, 1);

    for(int i = 0; i < n_plans; ++i) {
        int type;
        cin >> type;
        switch(type) {
            case 1:
                int v1, u1, w1;
                cin >> v1 >> u1 >> w1;
                --v1, --u1;
                plans[v1].push_back({u1, w1});
                break;
            case 2:
                int v2, l2, r2, w2;
                cin >> v2 >> l2 >> r2 >> w2;
                --v2, --l2, --r2;
                update_one_to_many_tree(one_to_many_offset, 0, n_planets - 1, 1, l2, r2, v2, w2);
                break;
            default:
                int v3, l3, r3, w3;
                cin >> v3 >> l3 >> r3 >> w3;
                --v3, --l3, --r3;
                update_many_to_one_tree(many_to_one_offset, 0, n_planets - 1, 1, l3, r3, v3, w3);
                break;
        }
    }

    cost_per_node = vector<ll>(n_nodes, LLONG_MAX);
    cost_per_node[e_idx] = 0;
    dijkstra(e_idx, n_nodes, plans, cost_per_node);

    for(int i = 0; i < n_planets; ++i) {
        if(cost_per_node[i] == LLONG_MAX) {
            cout << -1 << " ";
        } else {
            cout << cost_per_node[i] << " ";
        }
    }
}