#include <vector>
#include <iostream>

int getMaxCoins(const std::vector<int>& coin_vals) {
    std::vector<int> cur_best_combo{};
    int max_coins = 0;
    int combo_value = 0;

    for(auto coin : coin_vals) {
        if(combo_value < coin || max_coins == 0) {
            cur_best_combo.push_back(coin);
            ++max_coins;
            combo_value += coin;
        }
        else {
            combo_value -= cur_best_combo.back();
            cur_best_combo.pop_back();
            cur_best_combo.push_back(coin);
            combo_value += coin;
        }
    }
    return max_coins;
}

int main() {
    int cases;
    std::cin >> cases;
    for(int cur_case = 1; cur_case <= cases; ++cur_case) {
        int num_coins;
        std::cin >> num_coins;
        std::vector<int> coin_vals{};
        for(int i = 0; i < num_coins; ++i) {
            int cur_coin;
            std::cin >> cur_coin;
            coin_vals.push_back(cur_coin);
        }

        std::cout << getMaxCoins(coin_vals) << "\n";
    }
}