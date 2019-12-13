#include <iostream>
#include <string>
#include <sstream>

int main() {
    std::string input;
    while(std::getline(std::cin, input)) {
        std::string s, t;
        std::stringstream ss(input);
        ss >> s >> t;
        int s_idx = 0;
        for (auto t_ch : t) {
            if (t_ch == s[s_idx]) {
                ++s_idx;
                if (s_idx == s.size()) break;
            }
        }

        std::cout << (s_idx == s.size() ? "Yes\n" : "No\n");
    }
}