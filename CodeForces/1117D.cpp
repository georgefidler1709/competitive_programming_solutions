#include <bits/stdc++.h>

using namespace std;

/*
 * freddy frog
 */

const long long MOD = 1000000007;
const int MAX_GEM = 100;

// Implementation for square matrices.
struct Matrix {
    long long n;
    long long v[MAX_GEM][MAX_GEM];

    Matrix(int _n) : n(_n), v{0} {}

    Matrix operator*(const Matrix &o) const {
        Matrix res(n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                for (int k = 0; k < n; k++)
                    res.v[i][j] = (res.v[i][j] + v[i][k] * o.v[k][j]) % MOD;
        return res;
    }

    static Matrix getIdentity(int n) {
        Matrix res(n);
        for (int i = 0; i < n; i++) res.v[i][i] = 1;
        return res;
    }
    Matrix operator^(long long k) const {
        Matrix res = Matrix::getIdentity(n);
        Matrix a = *this;
        while (k) {
            if (k&1) res = res*a;
            a = a*a;
            k /= 2;
        }
        return res;
    }
};

int main() {
    long long n, m;
    scanf("%lld%lld", &n, &m);
    Matrix rec(MAX_GEM);

    rec.v[0][0] = 1;
    rec.v[0][m - 1] = 1;

    for (int i = 1; i < MAX_GEM; i++) {
        rec.v[i][i-1] = 1;
    }

    rec = rec^n;
    // The first entry of rec^N multiplied by (1,0,...,0)^T
    printf("%lld\n", rec.v[0][0]);
}