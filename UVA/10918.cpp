#include <bits/stdc++.h>

using namespace std;

constexpr int MAXN = 30;

using ll = long long;

ll send[MAXN + 1];
ll no_send[MAXN + 1];



int main() {
    //ios_base::sync_with_stdio(false);
    //cin.tie(NULL);

    int n;
    while(cin >> n, n != -1) {
        fill(send, send + MAXN, 0);
        fill(no_send, no_send + MAXN, 0);
        if(n % 2 == 1) {
            cout << "0" << "\n";
            continue;
        }
        no_send[0] = 1;
        no_send[2] = 3;
        send[2] = 2;
        for(int i = 4; i <= n; i += 2) {
            no_send[i] = 3 * no_send[i - 2] + send[i - 2];
            send[i] = 2 * no_send[i - 2] + send[i - 2];
        }

        cout << no_send[n] << "\n";

    }


}
