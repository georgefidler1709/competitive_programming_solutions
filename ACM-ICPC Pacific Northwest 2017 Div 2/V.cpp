//
// Created by hughe on 10/12/2019.
//

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct op {
    int add;
    ll x;
    char c;
};

void read_test(vector<op> & p1) {
    char c; cin >> c;
    while (c != 'E') {
        // first program
        if (c == 'I') {
            ll x; char L; cin >> x >> L;
            p1.push_back({1, x, L});
        } else /* c == 'D' */ {
            ll x; cin >> x;
            p1.push_back({0, x,'_'});
        }
        cin >> c;
    }
}

void bubble_sort(vector<op> & p1) {
    if (p1.size() <= 1) return; // early exit

    int N = (int)p1.size();

    bool run = true;
    while(run) {
        run = false;
        for (int i = 0; i < N-1; i++) {
            int j = i + 1;
            if (p1[i].add && p1[j].add) { // ADD and ADD
                // [I 1][I 2] => [I 1][I 2] : already ascending order
                // [I 1][I 1] => [I 1][I 2] : add to the first [I 1] before swapping as a [I 1] was after it
                // [I 2][I 1] => [I 1][I 3] : add to the [I 2] before swapping as a [I 1] was after it
                // so for [I X][I Y]: we increment X and swap iff X >= Y
                if (p1[i].x >= p1[j].x) {
                    p1[i].x++;
                    swap(p1[i],p1[j]);
                    run = true;
                }
            } else if (!p1[i].add && !p1[j].add) { // DELETE and DELETE
                // [D 1][D 2] => [D 3][D 1] : add to [D 2] before swapping as [D 1] was before it
                // [D 2][D 2] => [D 3][D 2] : add to the second [D 2]  before swapping as a [D 2] was before it
                // [D 2][D 1] => [D 2][D 1] : already descending order
                // so for [D X][D Y]: we increment Y and swap iff X <= Y
                if (p1[i].x <= p1[j].x) {
                    p1[j].x++;
                    swap(p1[i],p1[j]);
                    run = true;
                }
            } else if (!p1[i].add && p1[j].add) { // DELETE and ADD
                // [D 1][I 1] => [I 1][D 2] : add to the [D 1] before swapping as [I 1] was after it
                // [D 1][I 2] => [I 3][D 1] : add to the [I 2] before swapping as [D 1] was before it
                // [D 2][I 1] => [I 1][D 3] : add to the [D 2] before swapping as [I 1] was after it
                // [D X][I Y] : we increment X before swapping iff X >= Y
                // and increment Y before swapping if X < Y
                if (p1[i].x < p1[j].x) {
                    p1[j].x++; // increase add
                    swap(p1[i],p1[j]);
                    run = true;
                } else {
                    p1[i].x++; // increase del
                    swap(p1[i],p1[j]);
                    run = true;
                }
            }
        }
    }
}


void remove_redundant_pairs(vector<op> & p1) {
    if (p1.size() <= 1) return;

    int N = (int)p1.size();

    vector<bool> idx_to_remove(N, false);
    int i = 0; int j = N - 1;
    ll bonus = 0ll;
    while(i < N && j >= 0 && p1[i].add && !p1[j].add) {
        if (p1[i].x == p1[j].x) {
            // remove both i and j from vector
            idx_to_remove[i] = true;
            idx_to_remove[j] = true;
            bonus++;
            i++; j--;
        } else if (p1[i].x < p1[j].x) {
            p1[i].x -= bonus;
            i++;
        } else {
            p1[j].x -= bonus;
            j--;
        }
    }

    // tidy up shifts

    while(i < N && p1[i].add) {
        p1[i].x -= bonus;
        i++;
    }

    while(j >= 0 && !p1[j].add) {
        p1[j].x -= bonus;
        j--;
    }

    for (int i=N-1; i >= 0; i--) {
        // erase from end to prevent mutation.
        if (idx_to_remove[i]) {
            p1.erase(p1.begin() + i);
        }
    }
}



void minimise_inserts(vector<op> & p1) {
    if (p1.size() <= 1) return;

    int N = (int)p1.size();

    int i = 0; int j = N - 1;

    while(i < N && j >= 0 && p1[i].add && !p1[j].add) {

        while (i < N && p1[i].add) {
            // look for 1 above
            if (p1[i].x == p1[j].x+1) {
                // found - pull down and push up
                p1[i].x--;
                p1[j].x++;
                break;
            } else if (p1[i].x > p1[j].x+1) {
                // not found - continue
                break;
            }
            // try next i
            i++;
        }

        // try next j
        j--;
    }
}


int main() {

    vector<op> p1;
    vector<op> p2;

    read_test(p1);
    read_test(p2);

    int N = (int)p1.size();

    bubble_sort(p1);
    bubble_sort(p2);

    remove_redundant_pairs(p1);
    remove_redundant_pairs(p2);

    for(int i=0; i<N; i++) minimise_inserts(p1); // let's just do this in an n^2 way to be sure
    for(int i=0; i<N; i++) minimise_inserts(p2); // let's just do this in an n^2 way to be sure

    if (p1.size() != p2.size()) {
        cout << "1\n";
        return 0;
    }

    N = (int)p1.size();
    for (int i=0; i < N; i++) {
        if ((p1[i].add != p2[i].add) || (p1[i].x != p2[i].x) || (p1[i].c != p2[i].c)) {
            cout << "1\n";
            return 0;
        }
    }

    cout << "0\n";
    return 0;
}