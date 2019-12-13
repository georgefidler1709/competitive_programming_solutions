#include <iostream>
#include <string>

long long countPrefixes(std::string lower_bound, std::string upper_bound, long long num_strings, long long len) {

    // base case:
    // even if upper and lower bound are identical we still get 'len' prefixes
    long long prefixes = len;

    long long curr_lower = 0;
    long long curr_upper = 0;
    long long curr_index = 0;
    long long strings_used = 1;

    while(curr_index < len && strings_used < num_strings) {
        curr_lower = curr_lower << 1;
        curr_upper = curr_upper << 1;
        if(lower_bound[curr_index] == 'b') ++curr_lower;
        if(upper_bound[curr_index] == 'b') ++curr_upper;

        long long strings_found = curr_upper - curr_lower - strings_used + 1;
        if(strings_found + strings_used > num_strings) strings_found = num_strings - strings_used;

        strings_used += strings_found;
        prefixes += strings_found * (len - curr_index);
        ++curr_index;
    }

    return prefixes;

}

int main() {
    long long len, num_strings;
    std::cin >> len >> num_strings;
    std::string lower_bound;
    std::string upper_bound;
    std::cin >> lower_bound >> upper_bound;

    std::cout << countPrefixes(lower_bound, upper_bound, num_strings, len) << "\n";
}