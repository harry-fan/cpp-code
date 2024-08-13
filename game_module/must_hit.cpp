#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <random>

using namespace std;

struct HitConf {
    int id = 0;
    int upNo = 0;
    int weight = 0;
    vector<int> mustHits;
};

map<int, struct HitConf> datas;
void addConf(int id, int upNo, int weight) {
    struct HitConf tmp;
    tmp.id = id;
    tmp.upNo = upNo;
    tmp.weight = weight;
    datas[id] = tmp;
}

void loadDatas() {
    addConf(1, 150, 10);
    addConf(2, 0, 20);
    addConf(3, 0, 3);
    addConf(4, 0, 20);
    addConf(5, 0, 30);
    addConf(6, 0, 10);
    addConf(7, 0, 30);
    addConf(8, 0, 40);
    addConf(9, 0, 20);
    addConf(10, 0, 10);
    addConf(11, 0, 30);
    addConf(12, 0, 30);
    datas[3].mustHits = {10, 20, 30, 40, 50};
}
bool couldMustHit(const vector<int64_t> &uids, const struct HitConf &cfg);

int64_t random32(int64_t value) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, value);
    return dis(gen);
}

int64_t randomReward(const vector<int64_t> &uids) {
    int64_t totalWeight = 0;
    map<int64_t, int64_t> weights;
    int64_t drawTimes = uids.size() + 1;
    for (const auto &cfg : datas) {
        if (cfg.second.upNo && drawTimes < cfg.second.upNo) {
            continue;
        }
        if (couldMustHit(uids, cfg.second)) {
            return cfg.second.id;
        }
        totalWeight += cfg.second.weight;
        weights[cfg.second.id] = totalWeight;
    }
    auto randomValue = random32(totalWeight);
    for (const auto &weight : weights) {
        if (randomValue <= weight.second) {
            return weight.first;
        }
    }
    cout << "random error!" << endl;
}

bool couldMustHit(const vector<int64_t> &uids, const struct HitConf &cfg) {
    if (cfg.mustHits.empty()) {
        return false;
    }
    int64_t rangeMin = 0, drawTimes = uids.size() + 1;
    for (const auto &hit : cfg.mustHits) {
        if (drawTimes < hit) {
            continue;
        }
        auto start = uids.begin() + rangeMin;
        auto iter = std::find(start, uids.end(), cfg.id);
        if (iter == uids.end()) {
            return true;
        }
        rangeMin = hit;
    }
    return false;
}

int main() {
    vector<int64_t> uids;
    loadDatas();
    int idx = 100;
    while (--idx) {
        auto val = randomReward(uids);
        if (val) {
            uids.push_back(val);
        } else {
            cout << "random error! 2" << endl;
        }
    }
    for (int i = 0; i < uids.size(); ++i) {
        cout << i << "|" << uids.at(i) << endl;
    }
}
