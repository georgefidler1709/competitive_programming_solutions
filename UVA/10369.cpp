#include <bits/stdc++.h>

using namespace std;

using ll = long long;

double eucDist(pair<int,int>& a, pair<int,int>& b) {
    return sqrt((a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second));
}

void getDists(int n_posts, vector<pair<int,int>>& coords, vector<double>& dists) {
    vector<bool>included(n_posts, false);
    included[0] = true;
    dists[0] = 0;

    int u = 0;
    for(int i = 1; i < n_posts; ++i) {
        for(int v = 0; v < n_posts; ++v) {
            if(included[v]) continue;
            double dist = eucDist(coords[u], coords[v]);
            if(dist < dists[v]) {
                dists[v] = dist;
            }
        }
        int min_conn_idx = -1;
        for(int v = 0; v < n_posts; ++v) {
            if(included[v]) continue;
            if(min_conn_idx == -1 || dists[v] < dists[min_conn_idx]) {
                min_conn_idx = v;
            }
        }
        u = min_conn_idx;
        included[u] = true;
    }
}

int main() {
    int cases;
    cin >> cases;
    for(int c = 0; c < cases; ++c) {
        int sats, n_posts;
        cin >> sats >> n_posts;
        vector<pair<int,int>> coords; // x and y coords of outposts
        for(int p = 0; p < n_posts; ++p) {
            int x, y;
            cin >> x >> y;
            coords.push_back({x,y});
        }


        vector<double> dists(n_posts, INT_MAX);
        getDists(n_posts, coords, dists);
        sort(dists.begin(), dists.end());
        printf("%.2lf\n", dists[n_posts-sats]);
    }
}