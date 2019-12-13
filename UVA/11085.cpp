
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;
const int NUM_QUEENS = 8;

void flipConstraints(int row, int col, vector<vector<int>>& constraints, int flipVal) {
    int diag = 1;
    for(int i = row + 1; i < NUM_QUEENS; ++i) {
        constraints[i][col] += flipVal;
        if(col + diag < NUM_QUEENS) constraints[i][col + diag] += flipVal;
        if(col - diag >= 0)  constraints[i][col - diag] += flipVal;
        ++diag;
    }
}

void findAllArrangements(vector<int>& curr_queens, vector<vector<int>>& constraints, vector<vector<int>>& optimal_positions) {
    auto row = curr_queens.size();
    if(row == NUM_QUEENS) {
        optimal_positions.push_back(curr_queens);
        return;
    }

    for(int col = 0; col < NUM_QUEENS; ++col) {
        if(!constraints[row][col]) {
            curr_queens.push_back(col + 1);
            flipConstraints(row, col, constraints, 1);
            findAllArrangements(curr_queens, constraints, optimal_positions);
            flipConstraints(row, col, constraints, -1);
            curr_queens.pop_back();
        }
    }
}

int main() {
    string input;
    int num_case = 0;
    while(std::getline(std::cin, input)) {
        ++num_case;
        vector<vector<int>> constraints(NUM_QUEENS, vector<int>(NUM_QUEENS, 0));
        vector<vector<int>> optimal_positions;
        vector<int> curr_queens;
        vector<int>initial_pos;
        stringstream ss(input);

        int queen;
        while(ss >> queen) initial_pos.push_back(queen);

        findAllArrangements(curr_queens, constraints, optimal_positions);

        int min_moves = NUM_QUEENS;
        for(auto arr : optimal_positions) {
            int curr_moves = 0;
            for(int i = 0; i < NUM_QUEENS; ++i) {
                if(arr[i] != initial_pos[i]) ++curr_moves;
            }
            if(curr_moves < min_moves) min_moves = curr_moves;
        }

        cout << "Case " << num_case << ": " << min_moves << "\n";
    }
}