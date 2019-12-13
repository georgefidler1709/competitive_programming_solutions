#include <bits/stdc++.h>

using namespace std;
using ll = long long;

constexpr int MAXN = 200000;
using q = tuple<int, int, int>;
q queries[MAXN];

set<int>y_vals[MAXN];
int max_y[5 * MAXN];


// Procedure for recalculating a node's sum from its lazy and children.
void recalculate(int id, long long l, long long r) {
  if (r - l != 1) {
    max_y[id] = max(max_y[2 * id], max_y[2 * id + 1]);
  }
}
// The root node is responsible for [0, MAX_N). Update range [uL, uR)
void update_add(int x, int y, int i, int cLeft, int cRight) {
    if (cRight - cLeft == 1) {
        y_vals[x].insert(y);
        max_y[i] = (max_y[i] < y) ? y : max_y[i];
        return;
    }

    int mid = (cLeft + cRight) / 2;
    if (x < mid) update_add(x, y, i * 2, cLeft, mid);
    else update_add(x, y, i * 2 + 1, mid, cRight);

    recalculate(i, cLeft, cRight);
}

// The root node is responsible for [0, MAX_N). Update range [uL, uR)
void update_remove(int x, int y, int i, int cLeft, int cRight) {
    if (cRight - cLeft == 1) {
        y_vals[x].erase(y);
        if(y_vals[x].size() == 0) max_y[i] = -1;
        else max_y[i] = (max_y[i] == y) ? *(--y_vals[x].end()) : max_y[i];
        return;
    }
    int mid = (cLeft + cRight) / 2;
    if (x < mid) update_remove(x, y, i * 2, cLeft, mid);
    else update_remove(x, y, i * 2 + 1, mid, cRight);
    recalculate(i, cLeft, cRight);
}

int query(int x, int y, int i, int cLeft, int cRight) {
    // -1 necessary because cRight is exclusive
    if (cRight - 1 <= x) return -1;

    if(max_y[i] <= y) return -1;

    if(cRight - cLeft == 1) {
        return cLeft;
    }

    int mid = (cLeft + cRight) / 2;
    auto res = query(x, y, i * 2, cLeft, mid);
    if(res == -1) res = query(x, y, i * 2 + 1, mid, cRight);
    return res;
}

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n; cin >> n;

    for(int i = 0; i < n; ++i) {
        string q_type; cin  >> q_type;
        int x, y; cin >> x >> y;

        if(q_type == "add") {
            queries[i] = make_tuple(x, y, 0);
        } else if(q_type == "remove") {
            queries[i] = make_tuple(x, y, 1);
        } else {
            queries[i] = make_tuple(x, y, 2);
        }
    }

    int sorted_x[MAXN];
    for(int i = 0; i < n; ++i) {
        sorted_x[i] = get<0>(queries[i]);
    }

    sort(sorted_x, sorted_x + n);
    int num_uniq = (int)(unique(sorted_x, sorted_x + n) - sorted_x);

    for(int i = 0; i < n; ++i) {
        int x, y, q_type;
        tie(x, y, q_type) = queries[i];
        x = (int)(upper_bound(sorted_x, sorted_x + num_uniq, x) - sorted_x) - 1;
        switch(q_type) {
            case 0: // add
                update_add(x, y, 1, 0, num_uniq);
                break;
            case 1:
                update_remove(x, y, 1, 0, num_uniq);
                break;
            default:
                auto res = query(x, y, 1, 0, num_uniq);
                if(res == -1) cout << "-1" << "\n";
                else cout << sorted_x[res] << " " << *(y_vals[res].upper_bound(y)) << "\n";
        }
    }

}

