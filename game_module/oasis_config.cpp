#include <set>
#include <vector>
#include <map>
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

    // 开始match
    void startMatch();

    // 计算 mmrs
    map<int64_t, int64_t> calcMmrs();

    // match
    void stageMatch(vector<pair<int64_t, int64_t>> &mmrs);

    // 设置对手
    void setMatchResult(int64_t red, int64_t blue);

    // 获取legion数据
    struct OwActLegionInfo* getLegionObject(int64_t uid);

    // 获取对手
    int64_t findEnemy(int64_t redUid, const set<int64_t> &used, const vector<pair<int64_t, int64_t>> &mmrs);
};

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
 * legionx：备注
 * leagueuid: 100开头, 数据相同即为同一league
 * power: legionpower
 * server: 所属哪个服
 * legionuid：200开头, 必须不同
 * dan: 军团所在段位id
 * danScore: 军团所在段位分
 * winStr：x 指定连胜场次时，连败场次必须为0
 * loseStr：x 指定连败场次时，连胜场次必须为0
 * lastMatch：上次match的是哪个legion可以为0
 * signStage: signStage，相同时间段的legion会match在一起
 * */
static map<string, string> legionConfig = {
        make_pair("legion1", "leagueuid:100;power:10000;server:1;legionuid:200;dan:2;danScore:50;winStr:2;loseStr:0;lastMatch:0;signStage:1"),
        make_pair("legion2", "leagueuid:101;power:12000;server:2;legionuid:201;dan:2;danScore:60;winStr:1;loseStr:0;lastMatch:0;signStage:1"),
        make_pair("legion3", "leagueuid:102;power:14000;server:3;legionuid:202;dan:2;danScore:70;winStr:0;loseStr:1;lastMatch:0;signStage:1"),
        make_pair("legion4", "leagueuid:103;power:90000;server:4;legionuid:203;dan:2;danScore:80;winStr:1;loseStr:0;lastMatch:0;signStage:1"),
        make_pair("legion5", "leagueuid:104;power:7000;server:5;legionuid:204;dan:3;danScore:40;winStr:1;loseStr:0;lastMatch:0;signStage:1"),
        make_pair("legion6", "leagueuid:105;power:30000;server:6;legionuid:205;dan:3;danScore:30;winStr:0;loseStr:1;lastMatch:0;signStage:1"),
        make_pair("legion7", "leagueuid:106;power:70000;server:7;legionuid:206;dan:3;danScore:20;winStr:0;loseStr:1;lastMatch:0;signStage:1"),
        make_pair("legion8", "leagueuid:107;power:60000;server:8;legionuid:207;dan:3;danScore:19;winStr:1;loseStr:0;lastMatch:0;signStage:1"),
        make_pair("legion9", "leagueuid:108;power:19000;server:9;legionuid:208;dan:3;danScore:18;winStr:1;loseStr:0;lastMatch:0;signStage:1"),
        make_pair("legion10", "leagueuid:109;power:110000;server:10;legionuid:209;dan:3;danScore:17;winStr:3;loseStr:0;lastMatch:0;signStage:1"),
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
            std::cout << "parse " << info.first << " error1，请检查配置" << std::endl;
            exit(-1);
        }
        for (const auto &str : strs) {
            auto lstr = sepstr(str, ":");
            if (lstr.size() != 2) {
                std::cout << "parse " << info.first << " error2，请检查配置" << std::endl;
                exit(-1);
            }
            if (lstr.at(0) == "leagueuid") {
                legion.luid = stringToInt(lstr.at(1));
            } else if (lstr.at(0) == "power") {
                legion.power = stringToInt(lstr.at(1));
            } else if (lstr.at(0) == "server") {
                legion.server = stringToInt(lstr.at(1));
            } else if (lstr.at(0) == "legionuid") {
                legion.legUid = stringToInt(lstr.at(1));
            } else if (lstr.at(0) == "dan") {
                legion.danId = stringToInt(lstr.at(1));
            } else if (lstr.at(0) == "danScore") {
                legion.danScore = stringToInt(lstr.at(1));
            } else if (lstr.at(0) == "winStr") {
                legion.winStreak = stringToInt(lstr.at(1));
            } else if (lstr.at(0) == "loseStr") {
                legion.loseStreak = stringToInt(lstr.at(1));
            } else if (lstr.at(0) == "lastMatch") {
                legion.lastMatchUid = stringToInt(lstr.at(1));
            } else if (lstr.at(0) == "signStage") {
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
        cout << "stage match over|index:" << mitem.first << "|count:" << mitem.second.size() << std::endl;
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
        cout << "mmrs|legion:" << legion.first << "|dan:" << mmrDan << "|power:" << mmrPower << "|win:" << mmrWin
             << "|lose:" << mmrLose << endl;
        cout << "score|dan:" << legion.second.danId << "|power:" << legion.second.power
             << "|win:" << legion.second.winStreak << "|lose:" << legion.second.loseStreak
             << "|dest mmr:" << mmrs[legion.first] << std::endl;
    }
    return mmrs;
}

void oasis_match::stageMatch(vector<pair<int64_t, int64_t>> &mmrs) {
    if ((int64_t) mmrs.size() <= 1) {
        cout << "stage match less 1，no match" << std::endl;
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
        cout << "error:no valid op|red:" << redUid << "|blue:" << noInsUids.at(0) << endl;
        return noInsUids.at(0);
    }
    return 0;
}

void oasis_match::setMatchResult(int64_t red, int64_t blue) {
    auto legRed = getLegionObject(red);
    auto legBlue = getLegionObject(blue);
    if (nullptr == legRed || nullptr == legBlue) {
        cout << "error:set match legion not exist|red:" << red << "|blue:" << blue << endl;
        return;
    }
    legRed->opEnmy = blue;
    legBlue->opEnmy = red;
    legRed->lastMatchUid = blue;
    legBlue->lastMatchUid = red;
    cout << "match|red:" << red << "|server:" << legRed->server << "|league:" << legRed->luid << endl;
    cout << "match|blue:" << blue << "|server:" << legBlue->server << "|league:" << legBlue->luid << endl;
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
