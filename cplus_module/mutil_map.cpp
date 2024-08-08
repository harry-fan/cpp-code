#include <iostream>
#include <map>
#include <string>

int main() {
    std::multimap<int, std::string> infos;
    infos.insert(std::make_pair(1, "10000"));
    infos.insert(std::make_pair(1, "100000"));
    for (const auto &info : infos) {
        std::cout << info.first << "|" << info.second << std::endl;
    }

    auto iter = infos.find(1);
    if (iter != infos.end()) {
        // 获取首个数据
        std::cout << iter->second << std::endl;
    }

    auto range = infos.equal_range(1);
    // 获取多个数据
    for (auto itr = range.first; itr != range.second; ++itr) {
        std::cout << itr->first << "|" << itr->second << std::endl;
    }
}