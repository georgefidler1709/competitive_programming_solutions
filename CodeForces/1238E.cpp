// individual pairs in the string are really important
// like abcba is made up just of the pairs (ab) (bc)  e.g. as ab is equivalent to ba

#include <bits/stdc++.h>

using namespace std;

#define MAXN 100000
#define MAXM 20
#define INF 10000000

int adj[MAXM][MAXM];

int dp[1 << MAXM];


int main() {
    int n, m; cin >> n >> m;

    string s;
    cin >> s;

    // count character pairs in the string
    for(int i = 0; i < n - 1; ++i) {
        if(s[i] != s[i + 1]) { // don't need to move finger to type the same character so no cost
            ++adj[s[i] - 'a'][s[i + 1] - 'a'];
            ++adj[s[i + 1] - 'a'][s[i] - 'a'];
        }
    }

    int m_mask = 1<<m;
    // i = bitset of included characters
    for(int i = 1; i < m_mask; ++i) {
        dp[i] = INF;
        for(int j = 0; j < m; ++j) {
            int x = 1<<j;
            if(i & (1<<j)) { //if jth character is in the bitset i
                dp[i] = min(dp[i], dp[i ^ (1<<j)]); // take the cheapest version of i without 1 character, we will then readd that character
            }
        }
        for(int j = 0; j < m; ++j) {
            if(i & (1<<j)) { //if jth character is in the bitset i
                for(int k = 0; k < m; ++k) {
                    if(!(i & (1<<k))) {
                        dp[i] += adj[j][k]; // movement cost of the current set against character not added to the set yet (avoids double counting)
                    }
                }
            }
        }
    }

    cout << dp[m_mask - 1] << "\n";
}