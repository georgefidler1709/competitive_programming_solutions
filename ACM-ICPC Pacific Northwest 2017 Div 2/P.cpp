#include <iostream>
#include <cmath>

using namespace std;

int main() {
    
    long long a, b; cin >> a >> b;

    long long total = 0;
    long long bound = (long long) sqrt((double)b);

    for (long long i = 1; i <= bound; ++i) {

        // add the number of times i is a factor between the range, multiplied by i
        long long num = b/i - (a-1)/i;
        total += i * num;

        // add the factors that i divides to, starting from above the bound
        long long first = max(bound+1, a/i);
        if (first * i < a) first ++;
        long long last = b/i;
        long long between = min(last - first + 1, num);
        total += (between * (last + first)) / 2;
    }

    cout << total << endl;

    return 0;
}