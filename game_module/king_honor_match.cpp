//
// Created by Administrator on 2024/7/19.
//

#include <set>
#include <iostream>
#include <vector>

#define MATCH_BOTH 2

using namespace std;
int64_t getMinScoreSvr(const set<int64_t> &bronzeSvrs) {
    return bronzeSvrs.empty() ? 0 : *bronzeSvrs.begin();
}

void newServerMatch(const vector<int64_t> &newSvrs, set<int64_t> &bronzeSvrs) {
    if (newSvrs.empty()) {
        cout << "没有新加入的服，本周不进行新服匹配" << endl;
        return;
    }
    int64_t singleSvr = 0;
    auto iter = newSvrs.begin();
    if (newSvrs.size() % MATCH_BOTH != 0) {
        singleSvr = *iter++;
        bronzeSvrs.erase(singleSvr);
    }
    while (iter != newSvrs.end()) {
        auto red = *iter++;
        if (iter == newSvrs.end()) {
            cout << "迭代器越界，跳出循环" << endl;
            break;
        }
        auto blue = *iter++;
        cout << "match result:" << red << " - " << blue << endl;

        bronzeSvrs.erase(red);
        bronzeSvrs.erase(blue);
    }
    if (singleSvr) {
        auto svr = getMinScoreSvr(bronzeSvrs);
        if (svr) {
            bronzeSvrs.erase(svr);
            cout << "match result:" << singleSvr << " - " << svr << endl;
        }
    }
}

int main() {
    // 新服数据初始化
    vector<int64_t> newSvrs = {1, 2, 3, 4, 5};

    // 青铜服数据初始化
    set<int64_t> bronzeSvrs = {1, 2, 3, 4, 5, 6, 7, 8};

    // 新服匹配
    newServerMatch(newSvrs, bronzeSvrs);

    // 剩余的未匹配的服
    cout << "remain svrs:";
    for (const auto &svr : bronzeSvrs) {
        cout << svr << " ";
    }
    cout << endl;
}