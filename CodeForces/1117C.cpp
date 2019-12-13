#include <bits/stdc++.h>

using namespace std;

using ll = long long;

#define MAXN 100001
#define ITERATIONS 100
#define MAX 1e16

struct dist {
    int x;
    int y;
};

dist wind_dist[MAXN];

int main() {
    int src_x, src_y; cin >> src_x >> src_y;
    int dst_x, dst_y; cin >> dst_x >> dst_y;

    wind_dist[0] = {0,0};

    int n; cin >> n;

    for(int i = 1; i <= n; ++i) {
        char dir; cin >> dir;
        switch(dir) {
            case 'U':
                wind_dist[i] = {wind_dist[i - 1].x, wind_dist[i - 1].y + 1};
                break;
            case 'D':
                wind_dist[i] = {wind_dist[i - 1].x, wind_dist[i - 1].y - 1};
                break;
            case 'L':
                wind_dist[i] = {wind_dist[i - 1].x - 1, wind_dist[i - 1].y};
                break;
            default:
                wind_dist[i] = {wind_dist[i - 1].x + 1, wind_dist[i - 1].y};
        }
    }


    ll lo = 1;
    ll hi = 1e16;
    ll mid;
    ll best_so_far = -1;

    for(int i = 0; i < ITERATIONS; ++i) {
        mid = (hi + lo) / 2;
        ll cycles = mid / n;
        ll cycle_pos = mid % n;
        ll cur_x = src_x + wind_dist[cycle_pos].x + (wind_dist[n].x * cycles);
        ll cur_y = src_y + wind_dist[cycle_pos].y + (wind_dist[n].y * cycles);

        if(abs(dst_x - cur_x) + abs(dst_y - cur_y) <= mid) {
            best_so_far = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }

    if(best_so_far == -1) cout << "-1\n";
    else cout << best_so_far<< "\n";

}