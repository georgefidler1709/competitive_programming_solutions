#include <bits/stdc++.h>

using namespace std;

int main() {
    int n; cin >> n;
    int next = n + 1;
    
    string s = to_string(next);
    
    for(int i = 0; i < s.size(); ++i) {
        if(s[i] == '0') {
            for(; i < s.size(); ++i) s[i] = '1';
        }
    }
    
    cout << s << "\n";
}
