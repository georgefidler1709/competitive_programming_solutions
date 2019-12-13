#include <iostream>
#include <string>

using namespace std;

int main() {
    
    string s; cin >> s;
    
    bool foundPair = false;
    
    // if you find a pair of the same letter next to eachother, that is an even palindrom
    for (int i=0; i < s.length() - 1; i++){
        if (s[i] == s[i+1]) {
            foundPair = true;
            break;
        }
    }
    
    cout << (foundPair? "Or not." : "Odd.") << endl;
    
    return 0;
}