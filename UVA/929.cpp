#include <bits/stdc++.h>

using namespace std;

using ll = long long;

constexpr int MAX = 1000;
constexpr int INF = MAX * MAX * 9 + 1;

int maze[MAX][MAX];
vector<vector<int>>min_paths;

void dijkstra(int rows, int cols) {
    using node = pair<int, pair<int, int>>;

    priority_queue<node, vector<node>, greater<node>> pq;

    pq.push(node(maze[0][0], {0, 0}));
    min_paths[0][0] = maze[0][0];

    while (!pq.empty()) {
        int row = pq.top().second.first;
        int col = pq.top().second.second;
        pq.pop();

        int test_row = row;
        int test_col = col + 1;
        int test_w = maze[test_row][test_col];
        if(test_col < cols && min_paths[test_row][test_col] > min_paths[row][col] + test_w) {
            min_paths[test_row][test_col] = min_paths[row][col] + test_w;
            pq.push(node(min_paths[test_row][test_col], {test_row, test_col}));
        }

        test_row = row;
        test_col = col - 1;
        test_w = maze[test_row][test_col];
        if(test_col >= 0 && min_paths[test_row][test_col] > min_paths[row][col] + test_w) {
            min_paths[test_row][test_col] = min_paths[row][col] + test_w;
            pq.push(node(min_paths[test_row][test_col], {test_row, test_col}));
        }

        test_row = row + 1;
        test_col = col;
        test_w = maze[test_row][test_col];
        if(test_row < rows && min_paths[test_row][test_col] > min_paths[row][col] + test_w) {
            min_paths[test_row][test_col] = min_paths[row][col] + test_w;
            pq.push(node(min_paths[test_row][test_col], {test_row, test_col}));
        }

        test_row = row - 1;
        test_col = col;
        test_w = maze[test_row][test_col];
        if(test_row >= 0 && min_paths[test_row][test_col] > min_paths[row][col] + test_w) {
            min_paths[test_row][test_col] = min_paths[row][col] + test_w;
            pq.push(node(min_paths[test_row][test_col], {test_row, test_col}));
        }
    }
}



int main() {

    int mazes, rows, cols;

    cin >> mazes;

    for(int curr_case = 0; curr_case < mazes; ++curr_case) {
        cin >> rows >> cols;
        for(int row = 0; row < rows; ++row) {
            for(int col = 0; col < cols; ++col) {
                cin >> maze[row][col];
            }
        }

        min_paths = vector<vector<int>>(rows, vector<int>(cols, INF));
        min_paths[0][0] = maze[0][0];

        dijkstra(rows, cols);
        cout << min_paths[rows - 1][cols - 1] << "\n";
    }
}