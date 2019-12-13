#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int MAX_N = 36;

int main() {

    int N; cin >> N;

    vector<set<int, greater<>>> rows(N);
    vector<set<int, greater<>>> cols(N);

    bool isLatinSquare = true;
    bool isReduced = true;

    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {

            char c; cin >> c;

            if (c >= '0' && c <= '9') c = c - '0';
            if (c >= 'A' && c <= 'Z') c = c - 'A' + 10;

            int v; v = (int)c;

            if (i == 0 && j != v) isReduced = false;
            if (j == 0 && i != v) isReduced = false;

            if (rows[i].find(v) != rows[i].end()) isLatinSquare = false;
            if (cols[j].find(v) != cols[j].end()) isLatinSquare = false;

            rows[i].insert(v);
            cols[j].insert(v);

        }
    }

    cout << ( !isLatinSquare ? "No" : (!isReduced ? "Not Reduced" : "Reduced")) << "\n";

    return 0;
}


/*
11
0123456789Z
123456789A0
23456789A01
3456789A012
456789A0123
56789A01234
6789A012345
789A0123456
89A01234567
9A012345678
A0123456789
 */