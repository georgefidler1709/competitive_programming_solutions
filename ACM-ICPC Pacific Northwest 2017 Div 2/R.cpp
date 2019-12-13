#include <bits/stdc++.h>

using namespace std;

using ll = long long;

#define ITERATIONS 1000

#define PI 3.1415926535897932384626433

double  binarysearch(double k, double x) {
    double lo = -90.0;
    double hi = 90.0;
    double bestSoFar = -1;
    // Range [lo, hi];
    for(int j = 0; j < ITERATIONS; ++j) {
        double mid = (lo + hi) / 2;
        double y = k / cos(mid * PI / 180.0);// or something like that
        double target = x * tan(mid * PI / 180.0);
        if(y >= target) {
            bestSoFar = mid;
            lo = mid;
        } else {
            hi = mid;
            bestSoFar = mid;
        }
    }
    return bestSoFar;
}

int main() {

    int n, x;
    double v_robot;
    cin >> n >> x >> v_robot;


    double k = 0;
    for(int i = 0; i < n; ++i) {
        int l, r;
        double v;
        cin >> l >> r >> v;
        k += (r - l) * v;
        // do stuff to k
    }

    k /= v_robot;


    double angle = binarysearch(k, x);

    double hyp  = x / cos(angle * PI / 180.0);

    double time = hyp / v_robot;

    std::cout << std::fixed;
    std::cout << std::setprecision(3);

    double flat_time = 2 * (x/v_robot);

    if(time > flat_time) cout << "Too hard\n";
    else cout << time << "\n";


}
