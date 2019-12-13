// It's all about prime factorisation
// To square root something and get an integer prime factors must all be in sets of 2
// e.g. 2 * 2 * 3 * 3 = 36, sqrt(36) = 6 = 2 * 3
// So the smallest we can ever get a number with the 2 operations is to get all prime factors to be singular
// e.g. 20 = 5 * 2 * 2
// 1) multiply by 5: 100 = 5 * 5 * 2 * 2
// 2) sqrt(100) = 10 = 5 * 2
// To be able to srt again and get an integer we would have to multiply by 5 and then 2 first so we'd just be going in circles

#include <bits/stdc++.h>

using namespace std;

#define N 1e6

// Returns prime factors in increasing order with right multiplicity.
vector<int> primefactorize(int x) {
    vector<int> factors;
    for (int f = 2; f*f <= x; f++) {
        while (x % f == 0) {
            factors.push_back(f);
            x /= f;
        }
    }
    if (x != 1) factors.push_back(x);
    return factors;
}

int main() {
    int n; cin >> n;

    if(n == 1) {
        cout << "1 0\n";
        return 0;
    }

    int total = 1;
    int ops = 0;

    auto factors = primefactorize(n);
    unordered_map<int,int> fac_to_num;

    int max_fac = 0;
    for(const auto& f : factors) {
        if(fac_to_num.find(f) == fac_to_num.end()) {
            fac_to_num.insert({f, 1});
            total *= f;
        } else {
            ++fac_to_num[f];
        }
        if(fac_to_num[f] > max_fac) max_fac = fac_to_num[f];
    }

    if(max_fac != 1) {
        bool mult = false;
        int prev = -1;
        for (const auto &f : fac_to_num) {
            if (prev != -1 && f.second != prev) {
                mult = true;
                break;
            }
            prev = f.second;
        }

        int i;
        for (i = 1; i < max_fac; i <<= 1) ++ops;

        if (mult || max_fac != i) ++ops;
    }

    cout << total << " " << ops << "\n";
}