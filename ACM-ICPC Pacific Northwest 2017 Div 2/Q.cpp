#include <iostream>
#include <cmath>

using namespace std;

int main() {

    long long n; cin >> n;

    // they don't compare against themselves
    n --;

    long long total = n*(n+1)/2;

    // halfway of all the numbers multiplied
    long long halfway = (total % 2 == 0 ? total/2 : (total+1)/2);

    // find the dloat of what the answer should be
    long long bound = ((double) n) * (1.0 - 1.0/sqrt((double)2.0));

    // now check numbers near that float
    for (long long i = bound - 5; i < bound + 5; i++) {
        if (i*(2*n-i+1)/2 >= halfway) {
            cout << i << endl;
            break;
        }
    }
}