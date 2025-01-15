#include <iostream>
#include <cstdint>
#include <map>
#include <set>
#include <vector>
#include <cstdlib>
#include <algorithm>

using namespace std;
struct LeagueInfo;
map<int64_t, struct LeagueInfo> leagues;
void addConfig(int64_t luid, int64_t server, int64_t power, int64_t wzIdx, int64_t lastMatch);

// ------------------------------------- 配置区域 ----------------------------------
#define table_match_count 3

void loadConfig() {
    // 添加联盟信息 五个参数：
    // 第一个：联盟uid 不能为相同  必须有值
    // 第二个：联盟所在服         必须有值
    // 第三个：联盟无兵战力
    // 第四个：所在战区下标，100开始不可以和服一样。
    // 第五个：上次匹配的对手
    addConfig(1, 1, 1000000, 0, 0);
    addConfig(2, 2, 1200000, 0, 0);
    addConfig(13, 9, 1800000, 0, 0);
    addConfig(14, 1, 1800000, 0, 0);
    addConfig(15, 2, 21800000, 0, 0);
}

// ------------------------------------- 逻辑区域 ----------------------------------
struct LeagueInfo {
    int64_t luid = 0;
    int64_t server = 0;
    int64_t noSolPower = 0;
    int64_t warZoneIdx = 0;
    int64_t lastMatchUid = 0;
};

void addConfig(int64_t luid, int64_t server, int64_t power, int64_t wzIdx, int64_t lastMatch) {
    struct LeagueInfo info;
    info.luid = luid;
    info.server = server;
    info.noSolPower = power;
    info.warZoneIdx = wzIdx;
    info.lastMatchUid = lastMatch;
    leagues[luid] = info;
}

int64_t getLastMatchUid(int64_t luid) {
    auto iter = leagues.find(luid);
    return iter != leagues.end() ? iter->second.lastMatchUid : 0;
}

int64_t getServerId(int64_t luid) {
    auto iter = leagues.find(luid);
    return iter != leagues.end() ? iter->second.server : 0;
}

inline int32_t random32(int32_t min, int32_t max) {
    if (min == max) {
        return min;
    }
    if (min > max) {
        std::swap(min, max);
    }
    return rand() % (max - min + 1) + min;
}

template<class T>
inline vector<T> randomBatch(const set<T> &datas, int64_t m) {
    vector<T> selection;
    if (m <= 0) {
        return selection;
    }
    int64_t count = 0;
    for (const auto &data : datas) {
        if ((int64_t) selection.size() < m) {
            selection.push_back(data);
        } else {
            int64_t idx = random32(0, count);
            if (idx < m) {
                selection[idx] = data;
            }
        }
        count++;
    }
    return selection;
}

int64_t findEnemy(int64_t count, int64_t luid, const set<int64_t> &used,
                                     const vector<pair<int64_t, int64_t>> &mmrs) {
    set<int64_t> selects;
    set<int64_t> bakSelects;
    auto op = getLastMatchUid(luid);
    auto svr = getServerId(luid);
    for (const auto &mmr : mmrs) {
        if (used.find(mmr.first) != used.end()) {
            continue;
        }
        if (op == mmr.first) {
            bakSelects.insert(mmr.first);
            continue;
        }
        if (svr == getServerId(mmr.first)) {
            bakSelects.insert(mmr.first);
            continue;
        }
        if ((int64_t) selects.size() >= count) {
            break;
        }
        selects.insert(mmr.first);
    }
    if (selects.empty()) {
        return bakSelects.empty() ? 0 : randomBatch(bakSelects, 1).at(0);
    } else {
        return randomBatch(selects, 1).at(0);
    }
}

void randomMatchSelect(int64_t count, const vector<pair<int64_t, int64_t>> &mmrs) {
    set<int64_t> used;
    for (const auto &mmr : mmrs) {
        if (used.find(mmr.first) != used.end()) {
            continue;
        }
        auto red = mmr.first;
        used.insert(red);

        auto blue = findEnemy(count, red, used, mmrs);
        if (blue) {
            used.insert(blue);
        }
        cout << "match result|red:" << red << "|blue:" << blue << std::endl;
    }
}

void startMatch() {
    auto count = table_match_count;
    if (count <= 0) {
        cout << "match count error，need > 0" << count << std::endl;
        return;
    }
    if (leagues.empty()) {
        cout << "match league empty, not match!|cnt:" << leagues.size() << std::endl;
        return;
    }
    map<int64_t, vector<pair<int64_t, int64_t>>> spliteMmrs;
    for (const auto &league : leagues) {
        int64_t spkey = league.second.server;
        if (league.second.warZoneIdx) {
            spkey = league.second.warZoneIdx;
        }
        spliteMmrs[spkey].push_back(make_pair(league.first, league.second.noSolPower));
    }
    for (auto &smmr : spliteMmrs) {
        std::sort(smmr.second.begin(), smmr.second.end(),
                  [](const pair<int64_t, int64_t> &left, const pair<int64_t, int64_t> &right) {
                      if (left.second != right.second) {
                          return left.second > right.second;
                      }
                      return left.first < right.first;
                  });
        randomMatchSelect(count, smmr.second);
    }
}

int main() {
    srand(1);
    loadConfig();
    startMatch();
    return 0;
}