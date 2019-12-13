#include <bits/stdc++.h>

using namespace std;

#define N 1000001
#define K 10

// marked = not prime
bool marked[N+1];

void sieve() {
    for (int i = 2; i < N; i++) {
        if (!marked[i]) {
            for (int j = 2*i; j < N; j += i) {
                marked[j] = true;
                int tmp = j;
                while (tmp % i == 0) {
                    tmp /= i;
                }
            }
        }
    }
}

int kprimes[K];
int num_primes[N];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int cases; cin >> cases;
    sieve();
    for(int i = 2; i < N; ++i) {
        num_primes[i] = num_primes[i - 1] + (!marked[i]);
    }

    for(int t = 1; t <= cases; ++t) {
        int n, k; cin >> n >> k;

        for(int i = 0; i < k; ++i) {
           cin >> kprimes[i];
        }

        sort(kprimes, kprimes + k);
        for(int i = 0; i < k; ++i) {
            if(kprimes[i] > n) {
                k = i;
                break;
            }
        }

        int res = max(0, n - 1); // discard 1
        for(int i = 1; i < 1 << k; ++i) {
            int sign = 1;
            int lcm = 1;
            for(int j = 0; j < k && lcm <= n; ++j) {
                if((i >> j) & 1) {
                    lcm *= kprimes[j];
                    sign *= -1;
                }
            }

            int total = (n / lcm) * sign;
            res += total;
        }

        res -= (num_primes[n] - k);

        cout << "Case " << t << ": " << res << "\n";
    }

}