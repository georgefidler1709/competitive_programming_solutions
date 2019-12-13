#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int MAX_N = 10e5;

pair<int,int> dp[MAX_N][2]; /* <val, start> */

int main() {
    string s; cin >> s;
    int N = s.length();

    dp[0][0] = s[0] == 'R' ? make_pair(-1,0) : make_pair(1,0);
    dp[0][1] = s[0] == 'R' ? make_pair(1,0) : make_pair(-1,0);

    for (int i = 1; i < N; i++) {
        char c = s[i];
        if (c == 'R') {
//            dp[i][0] = max(dp[i-1][0]-1, -1);
//            dp[i][1] = max(dp[i-1][1]+1, 1);
            dp[i][0] = (dp[i-1][0].first-1 > -1) ? make_pair(dp[i-1][0].first-1, dp[i-1][0].second) : make_pair(-1, i);
            dp[i][1] = (dp[i-1][1].first+1 > 1) ? make_pair(dp[i-1][1].first+1, dp[i-1][1].second) : make_pair(1, i);
        } else if (c == 'B'){
//            dp[i][0] = max(dp[i-1][0]+1, 1);
//            dp[i][1] = max(dp[i-1][1]-1, -1);
            dp[i][0] = (dp[i-1][0].first+1 > 1) ? make_pair(dp[i-1][0].first+1, dp[i-1][0].second) : make_pair(1, i);
            dp[i][1] = (dp[i-1][1].first-1 > -1) ? make_pair(dp[i-1][1].first-1, dp[i-1][1].second) : make_pair(-1, i);
        }
    }

    int max_idx = -1;
    int max_diff = -2;
    int min_start = N;
    int isR = 0;
    for (int i = 0; i < N; i++) {
        if (dp[i][0].first > max_diff || (dp[i][0].first == max_diff && dp[i][0].second < min_start)) {
            // B
            max_idx = i;
            isR = 0;
            max_diff = dp[i][0].first;
            min_start = dp[i][0].second;
        }
        if (dp[i][1].first > max_diff || (dp[i][0].first == max_diff && dp[i][0].second < min_start)){
            // R
            max_idx = i;
            isR = 1;
            max_diff = dp[i][1].first;
            min_start = dp[i][1].second;
        }
    }

//    int i;
//    for (i = max_idx; i >= 0; i--) {
//        max_diff = max_diff + (s[i] == (isR? 'R' : 'B') ? -1 : 1);
//        if (max_diff == 0) break;
//    }

    cout << (min_start + 1) << " " << (max_idx + 1) << "\n";

    //

    return 0;
}