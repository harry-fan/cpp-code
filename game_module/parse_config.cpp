#include <iostream>
#include <sstream>
#include "../tutil/tc_common.h"
#include "../tutil/tc_config.h"

template<typename T>
vector<T> sepstr(const string &sStr, const string &sSep, bool withEmpty = false) {
    return tars::TC_Common::sepstr<T>(sStr, sSep, withEmpty);
}

template<typename T>
T strto(const string &sStr) {
    return tars::TC_Common::strto<T>(sStr);
}

bool isdigit(const string &sInput) {
    return tars::TC_Common::isdigit(sInput);
}

string serial(const vector<int64_t> &data) {
    std::stringstream ss;
    for (auto &v : data) {
        ss << v << ",";
    }
    return ss.str();
}

void parseData(const string &content) {
    tars::TC_Config config;
    config.parseString(content);

    map<int64_t, int64_t> setIds;
    map<int64_t, vector<int64_t>> idSets;
    for (const auto &item : config.getDomainMap("/SetRange")) {
        auto setData = sepstr<string>(item.first, "_");
        if (setData.size() != 2 || setData.at(0) != "set" || !isdigit(setData.at(1))) {
            cout << "input error|item:" << item.first << endl;
            continue;
        }
        auto setId = strto<int64_t>(setData.at(1));

        for (const auto &info : sepstr<string>(item.second, ",")) {
            if (isdigit(info)) {
                setIds[strto<int64_t>(info)] = setId;
                idSets[setId].push_back(strto<int64_t>(info));
                //cout << "setId:" << setId << "|server:" << strto<int64_t>(info) << endl;
            } else if (info.size() > 3 && *info.begin() == '[' && *info.rbegin() == ']') {
                // [1-100] 1到100服，步进固定为1
                auto rangeInfo = sepstr<int64_t>(info.substr(1, info.size() - 1), "-");
                if (rangeInfo.size() != 2 || rangeInfo.at(0) > rangeInfo.at(1)) {
                    cout << "parse error2|item:" << item.first << "|value:" << item.second  << endl;
                    continue;
                }
                for (auto serverId = rangeInfo.at(0); serverId <= rangeInfo.at(1); ++serverId) {
                    //cout << "setId:" << setId << "|server:" << serverId << endl;
                    setIds[serverId] = setId;
                    idSets[setId].push_back(serverId);
                }
            } else if (info.size() > 3 && *info.begin() == '(' && *info.rbegin() == ')') {
                // (1-100-2) 1到100服，步进为2
                auto rangeInfo = sepstr<int64_t>(info.substr(1, info.size() - 1), "-");
                if (rangeInfo.size() != 3 || rangeInfo.at(0) > rangeInfo.at(1) || rangeInfo.at(2) <= 0) {
                    cout << "parse error3|item:" << item.first << "|value:" << item.second  << endl;
                }
                for (auto serverId = rangeInfo.at(0); serverId <= rangeInfo.at(1); serverId += rangeInfo.at(2)) {
                    setIds[serverId] = setId;
                    idSets[setId].push_back(serverId);
                    //cout << "setId:" << setId << "|server:" << serverId << endl;
                }
            } else {
                cout << "parse error4|item:" << item.first << "|value:" << item.second  << endl;
            }
        }
    }
    for (const auto &s : setIds) {
        cout << "server:" << s.first << "|setId:" << s.second << endl;
    }
    for (const auto &s : idSets) {
        cout << "set:" << s.first << "|servers:" << serial(s.second) << endl;
    }
}

int main() {
    string config = "<SetRange>\n"
                    "\tset_0=(1-37-2),(49-97-4)\n"
                    "\tset_1=(2-38-2),(50-98-4)\n"
                    "\tset_2=(39-47-2),(51-99-4)\n"
                    "\tset_3=(40-48-2),(52-100-4)\n"
                    "</SetRange>";
    parseData(config);
}