//
// Created by Administrator on 2024/6/29.
//

#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdint>

using namespace std;

#define UNLOCK_MAX_DANID 5 // 当前解锁的最大段位id

namespace ptars {
    enum LMobilizeDan {
        LMobilizeDan_None       = 0,
        LMobilizeDan_NewBie     = 1,
        LMobilizeDan_Junior     = 2,
        LMobilizeDan_Elite      = 3,
        LMobilizeDan_Glory      = 4,
        LMobilizeDan_Legend     = 5,
    };
}

typedef struct LeagueInfo {
    int uid = 0;
    int curDanId = 0;
    int lastDanId = 0;
    int lscore = 0;
    int groupId = 0;
} LeagueInfo;


int64_t unlockMaxDanId = 3;     // 默认解锁最大的段位id
map<int, struct LeagueInfo> leagues;
void addLeague(int uid, int danId, int score, int64_t groupId) {
    struct LeagueInfo info;
    info.uid = uid;
    info.curDanId = danId;
    info.lastDanId = danId;
    info.lscore = score;
    info.groupId = groupId;
    leagues[info.uid] = info;
}

// 添加联盟
void loadLeagues() {
    // 联盟id 段位id 积分 分组id
    addLeague(1,1,8000,1);
    addLeague(2,1,8001,1);
    addLeague(3,1,8002,1);
    addLeague(4,1,8003,1);
    addLeague(5,1,8004,1);
    addLeague(6,1,8005,1);
    addLeague(7,1,8006,1);
    addLeague(8,1,8007,1);
    addLeague(9,1,8008,1);
    addLeague(10,1,8009,1);
    addLeague(11,1,8010,1);
    addLeague(12,1,8011,1);
    addLeague(13,1,8012,1);
    addLeague(14,1,8013,1);
    addLeague(15,1,8014,1);
    addLeague(16,1,8015,2);
    addLeague(17,1,8016,2);
    addLeague(18,1,8017,2);
    addLeague(19,1,8018,2);
    addLeague(20,1,8019,2);
    addLeague(21,1,8020,2);
    addLeague(22,1,8021,2);
    addLeague(23,1,8022,2);
    addLeague(24,1,8023,2);
    addLeague(25,1,8024,2);
    addLeague(26,1,8025,2);
    addLeague(27,1,8026,2);
    addLeague(28,1,8027,2);
    addLeague(29,1,8028,2);
    addLeague(30,1,8029,2);
    addLeague(31,1,8030,3);
    addLeague(32,1,8031,3);
    addLeague(33,1,8032,3);
    addLeague(34,1,8033,3);
    addLeague(35,1,8034,3);
    addLeague(36,1,8035,3);
    addLeague(37,1,8036,3);
    addLeague(38,1,8037,3);
    addLeague(39,1,8038,3);
    addLeague(40,1,8039,3);
    addLeague(41,1,8040,3);
    addLeague(42,1,8041,3);
    addLeague(43,1,8042,3);
    addLeague(44,1,8043,3);
    addLeague(45,1,8044,3);
    addLeague(46,1,8045,4);
    addLeague(47,1,8046,4);
    addLeague(48,1,8047,4);
    addLeague(49,1,8048,4);
    addLeague(50,1,8049,4);
    addLeague(51,1,8050,4);
    addLeague(52,1,8051,4);
    addLeague(53,1,8052,4);
    addLeague(54,1,8053,4);
    addLeague(55,1,8054,4);
    addLeague(56,1,8055,4);
    addLeague(57,1,8056,4);
    addLeague(58,1,8057,4);
    addLeague(59,1,8058,4);
    addLeague(60,1,8059,4);
    addLeague(61,2,8060,5);
    addLeague(62,2,8061,5);
    addLeague(63,2,8062,5);
    addLeague(64,2,8063,5);
    addLeague(65,2,8064,5);
    addLeague(66,2,8065,5);
    addLeague(67,2,8066,5);
    addLeague(68,2,8067,5);
    addLeague(69,2,8068,5);
    addLeague(70,2,8069,5);
    addLeague(71,2,8070,5);
    addLeague(72,2,8071,5);
    addLeague(73,2,8072,5);
    addLeague(74,2,8073,5);
    addLeague(75,2,8074,5);
    addLeague(76,2,8075,6);
    addLeague(77,2,8076,6);
    addLeague(78,2,8077,6);
    addLeague(79,2,8078,6);
    addLeague(80,2,8079,6);
    addLeague(81,2,8080,6);
    addLeague(82,2,8081,6);
    addLeague(83,2,8082,6);
    addLeague(84,2,8083,6);
    addLeague(85,2,8084,6);
    addLeague(86,2,8085,6);
    addLeague(87,2,8086,6);
    addLeague(88,2,8087,6);
    addLeague(89,2,8088,6);
    addLeague(90,2,8089,6);
    addLeague(91,2,8090,7);
    addLeague(92,2,8091,7);
    addLeague(93,2,8092,7);
    addLeague(94,2,8093,7);
    addLeague(95,2,8094,7);
    addLeague(96,2,8095,7);
    addLeague(97,2,8096,7);
    addLeague(98,2,8097,7);
    addLeague(99,2,8098,7);
    addLeague(100,2,8099,7);
    addLeague(101,2,8100,7);
    addLeague(102,3,8101,8);
    addLeague(103,3,8102,8);
    addLeague(104,3,8103,8);
    addLeague(105,3,8104,8);
    addLeague(106,3,8105,8);
    addLeague(107,3,8106,8);
    addLeague(108,3,8107,8);
    addLeague(109,3,8108,8);
    addLeague(110,3,8109,8);
    addLeague(111,3,8110,8);
    addLeague(112,3,8111,8);
    addLeague(113,3,8112,8);
    addLeague(114,3,8113,8);
    addLeague(115,3,8114,8);
    addLeague(116,3,8115,8);
    addLeague(117,3,8116,9);
    addLeague(118,3,8117,9);
    addLeague(119,3,8118,9);
    addLeague(120,3,8119,9);
    addLeague(121,3,8120,9);
    addLeague(122,3,8121,9);
    addLeague(123,3,8122,9);
    addLeague(124,3,8123,9);
    addLeague(125,3,8124,9);
    addLeague(126,3,8125,9);
    addLeague(127,3,8126,9);
    addLeague(128,3,8127,9);
    addLeague(129,3,8128,9);
    addLeague(130,3,8129,9);
    addLeague(131,3,8130,9);
    addLeague(132,3,8131,9);
    addLeague(133,3,8132,9);
    addLeague(134,3,8133,9);
    addLeague(135,3,8134,9);
    addLeague(136,3,8135,9);
}

struct TableConfig {
    int danId = 0;
    int upNum = 0;
    int downNum = 0;
    int upScore = 0;
    int unlockCnt = 0;
};

map<int, struct TableConfig> configs;
void addConfig(int danId, int up, int down, int upSocre, int unlockCnt) {
    struct TableConfig info;
    info.danId = danId;
    info.upNum = up;
    info.downNum = down;
    info.upScore = upSocre;
    info.unlockCnt = unlockCnt;
    configs[info.danId] = info;
}

// 添加配置
void loadConfig() {
    // 段位id 晋级数量 降级数量 晋级积分限制
    addConfig(1, 5, 0, 7000, 100);
    addConfig(2, 3, 3, 7000, 1000);
    addConfig(3, 3, 3, 7000, 1);
    addConfig(4, 3, 3, 7000, 1);
    addConfig(5, 3, 3, 7000, 100);
}

int64_t getNextDanId(int64_t curDanId, bool isUp = true) {
    switch (curDanId) {
        case ptars::LMobilizeDan_NewBie : {
            return isUp ? ptars::LMobilizeDan_Junior : ptars::LMobilizeDan_NewBie;
        }
        case ptars::LMobilizeDan_Junior : {
            return isUp ? ptars::LMobilizeDan_Elite : ptars::LMobilizeDan_NewBie;
        }
        case ptars::LMobilizeDan_Elite : {
            return isUp ? ptars::LMobilizeDan_Glory : ptars::LMobilizeDan_Junior;
        }
        case ptars::LMobilizeDan_Glory : {
            return isUp ? ptars::LMobilizeDan_Elite : ptars::LMobilizeDan_Legend;
        }
        case ptars::LMobilizeDan_Legend : {
            return isUp ? ptars::LMobilizeDan_Legend : ptars::LMobilizeDan_Glory;
        }
        default : {
            std::cout << "获取段位id错误|danId:" << curDanId << std::endl;
            return curDanId;
        }
    }
}

struct TableConfig getConfig(int danId) {
    return configs[danId];
}

struct UpDownItem {
    int64_t luid = 0;
    int64_t score = 0;
    int64_t danId = 0;
};

void checkUnlockMaxDan() {
    map<int64_t, int64_t> danCount;
    for (const auto &league : leagues) {
        danCount[league.second.curDanId] += 1;
    }
    if (!danCount.empty()) {
        auto maxDan = danCount.rbegin()->first;
        auto count = danCount.rbegin()->second;
        auto config = getConfig(maxDan);
        if (count > config.unlockCnt) {
            unlockMaxDanId = getNextDanId(unlockMaxDanId);
            cout << "unlock max danId|id:" << unlockMaxDanId << std::endl;
        }
    }
}

void tryUpDown() {
    map<int64_t, vector<struct UpDownItem>> sortScores;
    for (const auto &league : leagues) {
        auto &groupInfo = sortScores[league.second.groupId];
        struct UpDownItem item;
        item.luid = league.first;
        item.score = league.second.lscore;
        item.danId = league.second.curDanId;
        groupInfo.push_back(item);
    }
    bool needSave = false;
    for (auto &groupInfo : sortScores) {
        if (groupInfo.second.empty()) {
            continue;
        }
        std::stable_sort(groupInfo.second.begin(), groupInfo.second.end(),
                         [&](const struct UpDownItem &left, const struct UpDownItem &right){
                             if (left.score != right.score) {
                                 return left.score > right.score;
                             }
                             return left.luid < right.luid;
                         });

        int64_t rank = 0, crucial = 0;
        int64_t count = groupInfo.second.size();
        auto danId = groupInfo.second.at(0).danId;
        auto config = getConfig(danId);
        for (const auto &info : groupInfo.second) {
            ++rank;
            if (rank == config.upNum) {
                crucial = info.score;
            }
            if ((rank <= config.upNum || info.score >= crucial) && info.score >= config.upScore) {
                auto &league = leagues[info.luid];
                auto nextDanId = getNextDanId(danId);
                auto upDanId = std::min(nextDanId, unlockMaxDanId);
                if (upDanId != league.curDanId) {
                    needSave = true;
                    league.lastDanId = league.curDanId;
                    league.curDanId = upDanId;
                    cout << "up|luid:" << info.luid << "|old:" << league.lastDanId << "|cur:" << league.curDanId << std::endl;
                    // sendMail(info.luid, info.score, rank, upDanId, true);
                }
                continue;
            }
            if (rank + config.downNum > count) {
                auto &league = leagues[info.luid];
                auto nextDanId = getNextDanId(danId, false);
                if (nextDanId != league.curDanId) {
                    needSave = true;
                    league.lastDanId = league.curDanId;
                    league.curDanId = nextDanId;
                    cout << "down|luid:" << info.luid << "|old:" << league.lastDanId << "|cur:" << league.curDanId << std::endl;
                    // sendMail(info.luid, info.score, rank, nextDanId, false);
                }
            }
        }
    }
    if (needSave) {
//        setChange();
    }
}

int main() {
    loadConfig();
    loadLeagues();
    checkUnlockMaxDan();
    tryUpDown();
}