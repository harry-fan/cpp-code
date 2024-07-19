//
// Created by Administrator on 2024/6/27.
//
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h>

using namespace std;

#define DEF_GROUP_COUNT 15  // 默认每组联盟数量

typedef struct LeagueInfo {
    int uid = 0;
    int group = 0;
    int power = 0;
    int danId = 0;
    string name = "";
} LeagueInfo;

map<int, struct LeagueInfo> infos;
void addLeague(int uid, int power, int danId, const string &name) {
    struct LeagueInfo info;
    info.uid = uid;
    info.power = power;
    info.name = name;
    info.danId = danId;
    infos[info.uid] = info;
}

// 添加配置
void tryAddConfig() {
    addLeague(1, 1000, 1, "1盟");
    addLeague(2, 1001, 1, "2盟");
    addLeague(3, 1002, 1, "3盟");
    addLeague(4, 1003, 1, "4盟");
    addLeague(5, 1004, 1, "5盟");
    addLeague(6, 1005, 1, "6盟");
    addLeague(7, 1006, 1, "7盟");
    addLeague(8, 1007, 1, "8盟");
    addLeague(9, 1008, 1, "9盟");
    addLeague(10, 1009, 1, "10盟");
    addLeague(11, 1010, 1, "11盟");
    addLeague(12, 1011, 1, "12盟");
    addLeague(13, 1012, 1, "13盟");
    addLeague(14, 1013, 1, "14盟");
    addLeague(15, 1014, 2, "15盟");
    addLeague(16, 1015, 2, "16盟");
    addLeague(17, 1016, 2, "17盟");
    addLeague(18, 1017, 2, "18盟");
    addLeague(19, 1018, 2, "19盟");
    addLeague(20, 1019, 3, "20盟");
    addLeague(21, 1020, 3, "21盟");
    addLeague(22, 1021, 3, "22盟");
    addLeague(23, 1022, 3, "23盟");
    addLeague(24, 1023, 3, "24盟");
    addLeague(25, 1024, 1, "25盟");
    addLeague(26, 1025, 1, "26盟");
    addLeague(27, 1026, 1, "27盟");
    addLeague(28, 1027, 1, "28盟");
    addLeague(29, 1028, 1, "29盟");
    addLeague(30, 1029, 1, "30盟");
    addLeague(31, 1030, 1, "31盟");
    addLeague(32, 1031, 1, "32盟");
    addLeague(33, 1032, 1, "33盟");
    addLeague(34, 1033, 1, "34盟");
    addLeague(35, 1034, 1, "35盟");
    addLeague(36, 1035, 1, "36盟");
    addLeague(37, 1036, 1, "37盟");
    addLeague(38, 1037, 1, "38盟");
    addLeague(39, 1038, 1, "39盟");
    addLeague(40, 1039, 1, "40盟");
    addLeague(41,1039,1,"41盟");
    addLeague(42,1039,1,"42盟");
    addLeague(43,1039,1,"43盟");
    addLeague(44,1039,1,"44盟");
    addLeague(45,1039,1,"45盟");
    addLeague(46,1039,1,"46盟");
    addLeague(47,1039,1,"47盟");
    addLeague(48,1039,1,"48盟");
    addLeague(49,1039,1,"49盟");
    addLeague(50,1039,1,"50盟");
    addLeague(51,1039,1,"51盟");
    addLeague(52,1039,1,"52盟");
    addLeague(53,1039,1,"53盟");
    addLeague(54,1039,1,"54盟");
    addLeague(55,1039,1,"55盟");
    addLeague(56,1039,1,"56盟");
    addLeague(57,1039,1,"57盟");
    addLeague(58,1039,1,"58盟");
    addLeague(59,1039,1,"59盟");
    addLeague(60,1039,1,"60盟");
    addLeague(61,1039,1,"61盟");
    addLeague(62,1039,1,"62盟");
    addLeague(63,1039,1,"63盟");
    addLeague(64,1039,1,"64盟");
    addLeague(65,1039,1,"65盟");
    addLeague(66,1039,1,"66盟");
    addLeague(67,1039,1,"67盟");
    addLeague(68,1039,1,"68盟");
    addLeague(69,1039,1,"69盟");
    addLeague(70,1039,1,"70盟");
    addLeague(71,1039,2,"71盟");
    addLeague(72,1039,2,"72盟");
    addLeague(73,1039,2,"73盟");
    addLeague(74,1039,2,"74盟");
    addLeague(75,1039,2,"75盟");
    addLeague(76,1039,2,"76盟");
    addLeague(77,1039,2,"77盟");
    addLeague(78,1039,2,"78盟");
    addLeague(79,1039,2,"79盟");
    addLeague(80,1039,2,"80盟");
    addLeague(81,1039,2,"81盟");
    addLeague(82,1039,2,"82盟");
    addLeague(83,1039,2,"83盟");
    addLeague(84,1039,2,"84盟");
    addLeague(85,1039,2,"85盟");
    addLeague(86,1039,2,"86盟");
    addLeague(87,1039,2,"87盟");
    addLeague(88,1039,2,"88盟");
    addLeague(89,1039,2,"89盟");
    addLeague(90,1039,2,"90盟");
    addLeague(91,1039,2,"91盟");
    addLeague(92,1039,2,"92盟");
    addLeague(93,1039,2,"93盟");
    addLeague(94,1039,2,"94盟");
    addLeague(95,1039,2,"95盟");
    addLeague(96,1039,2,"96盟");
    addLeague(97,1039,2,"97盟");
    addLeague(98,1039,2,"98盟");
    addLeague(99,1039,2,"99盟");
    addLeague(100,1039,2,"100盟");
    addLeague(101,1039,2,"101盟");
    addLeague(102,1039,2,"102盟");
    addLeague(103,1039,2,"103盟");
    addLeague(104,1039,2,"104盟");
    addLeague(105,1039,2,"105盟");
    addLeague(106,1039,2,"106盟");
    addLeague(107,1039,3,"107盟");
    addLeague(108,1039,3,"108盟");
    addLeague(109,1039,3,"109盟");
    addLeague(110,1039,3,"110盟");
    addLeague(111,1039,3,"111盟");
    addLeague(112,1039,3,"112盟");
    addLeague(113,1039,3,"113盟");
    addLeague(114,1039,3,"114盟");
    addLeague(115,1039,3,"115盟");
    addLeague(116,1039,3,"116盟");
    addLeague(117,1039,3,"117盟");
    addLeague(118,1039,3,"118盟");
    addLeague(119,1039,3,"119盟");
    addLeague(120,1039,3,"120盟");
    addLeague(121,1039,3,"121盟");
    addLeague(122,1039,3,"122盟");
    addLeague(123,1039,3,"123盟");
    addLeague(124,1039,3,"124盟");
    addLeague(125,1039,3,"125盟");
    addLeague(126,1039,3,"126盟");
    addLeague(127,1039,3,"127盟");
    addLeague(128,1039,3,"128盟");
    addLeague(129,1039,3,"129盟");
    addLeague(130,1039,3,"130盟");
    addLeague(131,1039,3,"131盟");
    addLeague(132,1039,3,"132盟");
    addLeague(133,1039,3,"133盟");
    addLeague(134,1039,3,"134盟");
    addLeague(135,1039,3,"135盟");
    addLeague(136,1039,3,"136盟");
}

void tryDivideGroup() {
    if (infos.empty()) {
        cout << "没有联盟可参与" << std::endl;
        return;
    }

    map<int64_t, vector<struct LeagueInfo>> sortLeagueInfos;
    for (const auto &league : infos) {
        sortLeagueInfos[league.second.danId].push_back(league.second);
    }
    for (auto &sortInfo : sortLeagueInfos) {
        std::stable_sort(sortInfo.second.begin(), sortInfo.second.end(), 
                         [&](const struct LeagueInfo &left, const struct LeagueInfo &right) {
            if (left.power != right.power) {
                return left.power > right.power;
            }
            return left.uid < right.uid;
        });
    }
    auto count = DEF_GROUP_COUNT;
    int64_t remainBorder = ceil(count * 0.3);

    int groupMiddle = 0;
    for (auto &sortInfo : sortLeagueInfos) {
        int groupId = 0;
        int stat = 0, groupSize = sortInfo.second.size();
        auto maxGroupId = max(1, groupSize / count);
        if (groupSize % count > remainBorder) {
            maxGroupId = groupSize / count + 1;
        }
        for (const auto &league : sortInfo.second) {
            groupId = 1 + (stat / count);
            if (groupId > maxGroupId) {
                groupId = maxGroupId;
            }
            ++stat;
            groupId += groupMiddle;
            cout << "name:" << league.name << "|uid:" << league.uid << "|group:" << groupId << std::endl;
        }
        groupMiddle = std::max(groupId, groupMiddle);
        cout << "-------------" << sortInfo.first << "---------------" << std::endl;
    }
}

int main() {
    tryAddConfig();
    tryDivideGroup();
    return 0;
}