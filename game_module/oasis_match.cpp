#include "oasis_match.h"
#include <iostream>
#include <algorithm>

/**
 * factor配置
 * */
#define OasisWar_Match_Dan 50
#define OasisWar_Match_Power 10000
#define OasisWar_Match_Win 1000
#define OasisWar_Match_Lose 2000
#define OasisWar_Match_MaxWinLose 5000

/**
 * 军团x：备注
 * 联盟uid: 100开头, 数据相同即为同一联盟
 * 战力: 军团战力
 * 区服: 所属哪个服
 * 军团uid：200开头, 必须不同
 * 段位: 军团所在段位
 * 段位分: 军团的段位分
 * 连胜场数：x 指定连胜场次时，连败场次必须为0
 * 连败场数：x 指定连败场次时，连胜场次必须为0
 * 上次匹配军团uid：上次匹配的是哪个军团可以为0
 * 报名时间段: 报名时间段，相同时间段的军团会匹配在一起
 * */
static map<string, string> legionConfig = {
        make_pair("军团1", "联盟uid:100;战力:10000;区服:1;军团uid:200;段位:2;段位分:50;连胜场数:2;连败场数:0;上次匹配军团uid:0;报名时间段:1"),
        make_pair("军团2", "联盟uid:101;战力:12000;区服:2;军团uid:201;段位:2;段位分:60;连胜场数:1;连败场数:0;上次匹配军团uid:0;报名时间段:1"),
        make_pair("军团3", "联盟uid:102;战力:14000;区服:3;军团uid:202;段位:2;段位分:70;连胜场数:0;连败场数:1;上次匹配军团uid:0;报名时间段:1"),
        make_pair("军团4", "联盟uid:103;战力:90000;区服:4;军团uid:203;段位:2;段位分:80;连胜场数:1;连败场数:0;上次匹配军团uid:0;报名时间段:1"),
        make_pair("军团5", "联盟uid:104;战力:7000;区服:5;军团uid:204;段位:3;段位分:40;连胜场数:1;连败场数:0;上次匹配军团uid:0;报名时间段:1"),
        make_pair("军团6", "联盟uid:105;战力:30000;区服:6;军团uid:205;段位:3;段位分:30;连胜场数:0;连败场数:1;上次匹配军团uid:0;报名时间段:1"),
        make_pair("军团7", "联盟uid:106;战力:70000;区服:7;军团uid:206;段位:3;段位分:20;连胜场数:0;连败场数:1;上次匹配军团uid:0;报名时间段:1"),
        make_pair("军团8", "联盟uid:107;战力:60000;区服:8;军团uid:207;段位:3;段位分:19;连胜场数:1;连败场数:0;上次匹配军团uid:0;报名时间段:1"),
        make_pair("军团9", "联盟uid:108;战力:19000;区服:9;军团uid:208;段位:3;段位分:18;连胜场数:1;连败场数:0;上次匹配军团uid:0;报名时间段:1"),
        make_pair("军团10", "联盟uid:109;战力:110000;区服:10;军团uid:209;段位:3;段位分:17;连胜场数:3;连败场数:0;上次匹配军团uid:0;报名时间段:1"),
};

////////////////////////////////// 代码区域 ///////////////////////////////////////////////
int64_t stringToInt(const string &str) {
    try {
        int result = std::stoi(str);
        return result;
    } catch (const std::invalid_argument& e) {
        std::cout << "Error: The string \"" << str << "\" cannot be converted to an integer." << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << "Error: The string \"" << str << "\" is out of the integer range." << std::endl;
    }
    return 0;
}

vector<string> sepstr(const string &sStr, const string &sSep, bool withEmpty = false)
{
    vector<string> vt;

    string::size_type pos = 0;
    string::size_type pos1 = 0;

    while (true) {
        string s;
        pos1 = sStr.find_first_of(sSep, pos);
        if (pos1 == string::npos) {
            if (pos + 1 <= sStr.length()) {
                s = sStr.substr(pos);
            }
        }
        else if (pos1 == pos) {
            s = "";
        }
        else {
            s = sStr.substr(pos, pos1 - pos);
            pos = pos1;
        }

        if (withEmpty) {
            vt.push_back(std::move(s));
        }
        else {
            if (!s.empty()) {
                vt.push_back(std::move(s));
            }
        }

        if (pos1 == string::npos) {
            break;
        }

        pos++;
    }

    return vt;
}

void oasis_match::loadConfig() {
    for (const auto &info : legionConfig) {
        struct OwActLegionInfo legion;
        auto strs = sepstr(info.second, ";");
        if (strs.size() != 10) {
            std::cout << "解析 " << info.first << " 错误1，请检查配置" << std::endl;
            exit(-1);
        }
        for (const auto &str : strs) {
            auto lstr = sepstr(str, ":");
            if (lstr.size() != 2) {
                std::cout << "解析 " << info.first << " 错误2，请检查配置" << std::endl;
                exit(-1);
            }
            if (lstr.at(0) == "联盟uid") {
                legion.luid = stringToInt(lstr.at(1));
            } else if (lstr.at(0) == "战力") {
                legion.power = stringToInt(lstr.at(1));
            } else if (lstr.at(0) == "区服") {
                legion.server = stringToInt(lstr.at(1));
            } else if (lstr.at(0) == "军团uid") {
                legion.legUid = stringToInt(lstr.at(1));
            } else if (lstr.at(0) == "段位") {
                legion.danId = stringToInt(lstr.at(1));
            } else if (lstr.at(0) == "段位分") {
                legion.danScore = stringToInt(lstr.at(1));
            } else if (lstr.at(0) == "连胜场数") {
                legion.winStreak = stringToInt(lstr.at(1));
            } else if (lstr.at(0) == "连败场数") {
                legion.loseStreak = stringToInt(lstr.at(1));
            } else if (lstr.at(0) == "上次匹配军团uid") {
                legion.lastMatchUid = stringToInt(lstr.at(1));
            } else if (lstr.at(0) == "报名时间段") {
                legion.signUpIndex = stringToInt(lstr.at(1));
            }
        }
        data.legions[legion.legUid] = legion;
    }
}

void oasis_match::startMatch() {
    auto mmrs = calcMmrs();
    map<int64_t, vector<pair<int64_t, int64_t>>> matchs;
    for (const auto &legion : data.legions) {
        matchs[legion.second.signUpIndex].push_back(make_pair(legion.first, mmrs[legion.first]));
    }
    for (auto &mitem : matchs) {
        stageMatch(mitem.second);
        cout << "分段匹配结束|index:" << mitem.first << "|count:" << mitem.second.size() << std::endl;
    }
}

map<int64_t, int64_t> oasis_match::calcMmrs() {
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
        cout << "mmrs|军团:" << legion.first << "|段位分:" << mmrDan << "|战力分:" << mmrPower << "|胜利分:" << mmrWin
                            << "|失败分:" << mmrLose << "|段位:" << legion.second.danId << "|战力:" << legion.second.power
                            << "|胜场:" << legion.second.winStreak << "|败场:" << legion.second.loseStreak
                            << "|最终mmr分:" << mmrs[legion.first] << std::endl;
    }
    return mmrs;
}

void oasis_match::stageMatch(vector<pair<int64_t, int64_t>> &mmrs) {
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

int64_t oasis_match::findEnemy(int64_t redUid, const set<int64_t> &used,
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
        cout << "无有效对手填充|red:" << redUid << "|blue:" << noInsUids.at(0) << endl;
        return noInsUids.at(0);
    }
    return 0;
}

void oasis_match::setMatchResult(int64_t red, int64_t blue) {
    auto legRed = getLegionObject(red);
    auto legBlue = getLegionObject(blue);
    if (nullptr == legRed || nullptr == legBlue) {
        cout << "设置匹配结果军团不存在|red:" << red << "|blue:" << blue << endl;
        return;
    }
    legRed->opEnmy = blue;
    legBlue->opEnmy = red;
    legRed->lastMatchUid = blue;
    legBlue->lastMatchUid = red;
    cout << "匹配|红方:" << red << "|区服:" << legRed->server << "|联盟:" << legRed->luid << endl;
    cout << "匹配|蓝方:" << blue << "|区服:" << legBlue->server << "|联盟:" << legBlue->luid << endl;
    cout << "-------------------------------------------------------------------------------------" << endl;
}

struct OwActLegionInfo *oasis_match::getLegionObject(int64_t uid) {
    auto iter = data.legions.find(uid);
    return iter == data.legions.end() ? nullptr : &iter->second;
}

int main() {
    oasis_match::loadConfig();
    oasis_match::startMatch();
}