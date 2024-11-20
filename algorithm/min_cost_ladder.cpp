//
// Created by Administrator on 2024/9/26.
//

#include <iostream>
#include <vector>

using namespace std;

/* 最小花费爬楼梯算法
 * cost = [10, 15, 20] cost=15, 从15直接跳过2步出来
 * cost = [1, 100, 1, 1, 1, 100, 1, 1, 100, 1] cost=6, 从第一步开始挑出来最大cost=6
 * */
int cost_ladder(const vector<int> &costs) {
    if (costs.size() <= 2) {
        return 0;
    }
    vector<int> dp(costs.size() + 1, 0);
    for (int i = 2; i <= costs.size(); ++i) {
        dp[i] = std::min(dp[i - 1] + costs[i - 1], dp[i - 2] + costs[i - 2]);
    }
    return dp[costs.size()];
}

int main() {
    vector<int> data = {1, 100, 1, 1, 1, 100, 1, 1, 100, 1};
    cout << cost_ladder(data) << endl;
}