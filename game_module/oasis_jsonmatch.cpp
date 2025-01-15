
#include "oasis_match.h"
#include <iostream>
#include <algorithm>
#include "../tutil/tc_common.h"
#include "../tutil/tc_file.h"
#include "../third/nlohmann_json/json.hpp"

/**
 * factor配置
 * */
#define OasisWar_Match_Dan 50
#define OasisWar_Match_Power 30000
#define OasisWar_Match_Win 1000
#define OasisWar_Match_Lose 2000
#define OasisWar_Match_MaxWinLose 5000

////////////////////////////////// 代码区域 ///////////////////////////////////////////////
int64_t strtoint64(const string &str) {
    return tars::TC_Common::strto<int64_t>(str);
}

vector<string> sepstr(const string &str, const string &sep) {
    return tars::TC_Common::sepstr<string>(str, sep);
}

void loadConfig(const string &jsonData) {
    auto jsoninfo = nlohmann::json::parse(jsonData);
    for (const auto &legion : jsoninfo["legions"]) {
        auto legionUid = strtoint64(legion["legion_id"].get<string>());
        if (legionUid == 5143379463282675712 || legionUid == 5143403317128384512) {
            continue;
        }

        struct OwActLegionInfo actInfo;
        actInfo.luid = strtoint64(legion["union_id"].get<string>());
        actInfo.server = legion["server_id"].get<int64_t>();
        actInfo.power = legion["match_power"].get<int64_t>();
        actInfo.legUid = legionUid;
        actInfo.danId = legion["lv"].get<int64_t>();
        actInfo.winStreak = legion["lv"].get<int64_t>();
        actInfo.loseStreak = legion["loseing_streak"].get<int64_t>();
        actInfo.signUpIndex = legion["sign_index"].get<int64_t>();
        actInfo.sid = legion["legion_sort"].get<int64_t>();
        actInfo.leagueName = legion["union_name"].get<string>();
        data.legions[legionUid] = actInfo;
    };
}

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

void startMatch() {
    auto mmrs = calcMmrs();
    map<int64_t, vector<pair<int64_t, int64_t>>> matchs;
    for (const auto &legion : data.legions) {
        matchs[legion.second.signUpIndex].push_back(make_pair(legion.first, mmrs[legion.first]));
    }
    for (auto &mitem : matchs) {
        stageMatch(mitem.second);
        cout << "分段匹配结束,index:" << mitem.first << ",count:" << mitem.second.size() << std::endl;
    }
}

map<int64_t, int64_t> calcMmrs() {
    map<int64_t, int64_t> mmrs;
    auto danRate = OasisWar_Match_Dan;
    auto powerRate = OasisWar_Match_Power;
    auto winRate = OasisWar_Match_Win;
    auto loseRate = OasisWar_Match_Lose;
    auto maxWinLoseMmr = OasisWar_Match_MaxWinLose;
    powerRate = powerRate == 0 ? 10000.0 : powerRate;
    for (const auto &legion : data.legions) {
        auto mmrDan = legion.second.danScore * danRate;
        int64_t mmrPower = 1.0 * legion.second.power / powerRate;
        auto mmrWin = legion.second.winStreak * winRate;
        auto mmrLose = legion.second.loseStreak * loseRate;
        mmrWin = mmrWin > maxWinLoseMmr ? maxWinLoseMmr : mmrWin;
        mmrLose = mmrLose > maxWinLoseMmr ? maxWinLoseMmr : mmrLose;
        mmrs[legion.first] = mmrDan + mmrPower + mmrWin - mmrLose;
        cout << "mmrs,军团:" << legion.first << ",段位分:" << mmrDan << ",战力分:" << mmrPower << ",胜利分:" << mmrWin
             << ",失败分:" << mmrLose << ",段位:" << legion.second.danId << ",战力:" << legion.second.power
             << ",胜场:" << legion.second.winStreak << ",败场:" << legion.second.loseStreak
             << ",最终mmr分:" << mmrs[legion.first] << std::endl;
    }
    return mmrs;
}

// 军团,段位分,战力分,胜利分,失败分,段位,战力,胜场,败场
void updateConfig() {
    std::string filename = "oasis_config.txt";
    // 打开文件
    std::ifstream file(filename);
    // 检查文件是否成功打开
    if (!file.is_open()) {
        std::cout << "无法打开文件: " << filename << std::endl;
        return;
    }
    // 逐行读取
    std::string line;
    while (std::getline(file, line)) {
        auto lineInfos = sepstr(line, ",");
        if (lineInfos.size() != 9) {
            cout << "配置错误:" << line << endl;
            continue;
        }
        auto legionUid = strtoint64(lineInfos[0]);
        auto danScore = strtoint64(lineInfos[5]);
        auto iter = data.legions.find(legionUid);
        if (iter != data.legions.end()) {
            iter->second.danScore = danScore;
            cout << "更新段位分：" << legionUid << "|" << danScore << endl;
        }
    }
}

void stageMatch(vector<pair<int64_t, int64_t>> &mmrs) {
    if ((int64_t) mmrs.size() <= 1) {
        cout << "阶段军团小于1个，不匹配" << std::endl;
        return;
    }
    std::sort(mmrs.begin(), mmrs.end(),
              [](const pair<int64_t, int64_t> &left, const pair<int64_t, int64_t> &right) {
                  if (left.second != right.second) {
                      return left.second > right.second;
                  }
                  return left.first > right.first;
              });
    set<int64_t> used;
    for (auto iter = mmrs.begin(); iter != mmrs.end(); ++iter) {
        if (used.find(iter->first) != used.end()) {
            continue;
        }
        auto red = iter->first;
        used.insert(red);

        auto blue = findEnemy(red, used, mmrs);
        if (blue) {
            used.insert(blue);
        }
        // 设置对手
        setMatchResult(red, blue);
    }
}

int64_t findEnemy(int64_t redUid, const set<int64_t> &used,
                               const vector<pair<int64_t, int64_t>> &mmrs) {
    auto legionRed = getLegionObject(redUid);
    if (nullptr == legionRed) {
        return 0;
    }
    vector<int64_t> noInsUids;
    for (const auto &mmr : mmrs) {
        if (used.find(mmr.first) != used.end()) {
            continue;
        }
        auto legionBlue = getLegionObject(mmr.first);
        if (nullptr == legionBlue) {
            continue;
        }
        if (legionRed->luid == legionBlue->luid) {
            continue;
        }
        if (legionRed->server == legionBlue->server) {
            noInsUids.push_back(mmr.first);
            continue;
        }
        if (legionRed->lastMatchUid == mmr.first) {
            noInsUids.push_back(mmr.first);
            continue;
        }
        return mmr.first;
    }
    if (!noInsUids.empty()) {
        cout << "无有效对手填充,red:" << redUid << ",blue:" << noInsUids.at(0) << endl;
        return noInsUids.at(0);
    }
    return 0;
}

void setMatchResult(int64_t red, int64_t blue) {
    auto legRed = getLegionObject(red);
    auto legBlue = getLegionObject(blue);
    if (nullptr == legRed || nullptr == legBlue) {
        cout << "设置匹配结果军团不存在,red:" << red << ",blue:" << blue << endl;
        return;
    }
    legRed->opEnmy = blue;
    legBlue->opEnmy = red;
    legRed->lastMatchUid = blue;
    legBlue->lastMatchUid = red;
    cout << "匹配,红方:" << red << ",区服:" << legRed->server << ",联盟名字:" << legRed->leagueName << ",第x军团:" << legRed->sid << ",联盟:" << legRed->luid << endl;
    cout << "匹配,蓝方:" << blue << ",区服:" << legBlue->server << ",联盟名字:" << legBlue->leagueName << ",第x军团:" << legBlue->sid << ",联盟:" << legBlue->luid << endl;
    cout << "-------------------------------------------------------------------------------------" << endl;
}

struct OwActLegionInfo *getLegionObject(int64_t uid) {
    auto iter = data.legions.find(uid);
    return iter == data.legions.end() ? nullptr : &iter->second;
}

int main() {
    if (!tars::TC_File::isFileExist("data.json")) {
        cout << "json文件不存在" << endl;
        return 0;
    }
    auto info = tars::TC_File::load2str("data.json");
    loadConfig(info);
    updateConfig();
    startMatch();
}