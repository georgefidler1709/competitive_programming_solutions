#include <bits/stdc++.h>

using namespace std;


struct jewel {
    int x, y, c;
};

bool operator < (const jewel& a, const jewel& b) {
    return a.y < b.y;
}

const int MAX_N = 200000;
int x_vals[MAX_N], least_sig_bits[MAX_N];
jewel jewels[MAX_N];
vector<set<int>> range_per_colour(MAX_N, set<int>());


int sum(int x) {
    int result = 0;
    for( ; x > 0; x -= x & -x)
        result += least_sig_bits[x];
    return result;
}

int main() {
    int cases;
    cin >> cases;

    for(int cur_case = 0; cur_case < cases; ++cur_case) {
        memset(least_sig_bits, 0, sizeof least_sig_bits);
        int n_jewels, n_colours;
        cin >> n_jewels >> n_colours;

        for(int i = 0; i < n_jewels; ++i) {
            int x, y, c;
            cin >> x >> y >> c;
            --c;
            jewels[i] = {x,y,c};
            x_vals[i] = jewels[i].x;
        }
        sort(x_vals, x_vals + n_jewels);

        // reduce x values to index order e.g. [2,5,9] to [1,2,3]
        int uniq_x_vals = unique(x_vals, x_vals + n_jewels) - x_vals;
        for(int i = 0; i < n_jewels; ++i)
            jewels[i].x = lower_bound(x_vals, x_vals + uniq_x_vals, jewels[i].x) - x_vals + 1;

        sort(jewels, jewels + n_jewels);

        for(int i = 0; i < n_colours; ++i) {
            range_per_colour[i].clear();
            range_per_colour[i].insert(0);
            range_per_colour[i].insert(uniq_x_vals + 1);
        }

        int result = 0;
        int i = 0;
        int j = 0;
        while(i < n_jewels) {
            for(j = i; j < n_jewels && jewels[i].y == jewels[j].y; ++j) {  // looking at every jewel in the same row
                auto it = range_per_colour[jewels[j].c].lower_bound(jewels[j].x);
                int r = *it - 1;
                int l = *(--it);
                int curr =  sum(r) - sum(l);
                if (result <  curr) result = curr;
            }
            for(; i < j; ++i) { //count for that row
                int curr_x = jewels[i].x;
                for(; curr_x <= uniq_x_vals; curr_x += curr_x & -curr_x) ++least_sig_bits[curr_x];
                range_per_colour[jewels[i].c].insert(jewels[i].x);
            }
        }
        for(i = 0; i < n_colours; ++i) { // check for overlap
            int prev = 0;
            for(auto it = ++range_per_colour[i].begin(); it != range_per_colour[i].end(); ++it) {
                int curr = sum(*it - 1) - sum(prev);
                if (result < curr) result = curr;
                prev = *it;
            }
        }
        cout << result << "\n";
    }
}

