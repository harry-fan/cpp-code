#ifndef CPP_CODE_OASIS_MATCH_H
#define CPP_CODE_OASIS_MATCH_H

#include <set>
#include <vector>
#include <map>
#include <string>
#include <cstdint>

using namespace std;
struct OwActLegionInfo {
    int64_t luid = 0;
    int64_t power = 0;
    int64_t server = 0;
    int64_t opEnmy = 0;
    int64_t legUid = 0;
    int64_t danId = 0;
    int64_t danScore = 0;
    int64_t winStreak = 0;
    int64_t loseStreak = 0;
    int64_t lastMatchUid = 0;
    int64_t signUpIndex = 0;
};

struct OwCenterData {
    map<int64_t, struct OwActLegionInfo> legions;
};

struct OwCenterData data;

using namespace std;
namespace oasis_match {
    // 获取配置
    void loadConfig();

    // 开始匹配
    void startMatch();

    // 计算 mmrs
    map<int64_t, int64_t> calcMmrs();

    // 匹配
    void stageMatch(vector<pair<int64_t, int64_t>> &mmrs);

    // 设置对手
    void setMatchResult(int64_t red, int64_t blue);

    // 获取军团数据
    struct OwActLegionInfo* getLegionObject(int64_t uid);

    // 获取对手
    int64_t findEnemy(int64_t redUid, const set<int64_t> &used, const vector<pair<int64_t, int64_t>> &mmrs);
};


#endif //CPP_CODE_OASIS_MATCH_H
