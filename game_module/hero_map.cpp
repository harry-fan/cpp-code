
#include <iostream>
#include <map>
#include <vector>

using namespace std;
static std::vector<pair<int, std::string>> timeSegments = {
        {10, "Item1"},
        {20, "Item2"},
        {30, "Item3"}
};

int getNextIndex(int lastIdx) {
    int size = timeSegments.size();
    if (!lastIdx || lastIdx == size) {
        return 0;
    }
    return ++lastIdx;
}

std::map<std::string, int> calcHookDropItem(int lstIdx, int valueTime) {
    std::map<std::string, int> items;

    int maxDropTm = 30;
    int newIndex = 0, nextIndex = getNextIndex(lstIdx);
    while (valueTime > 0) {
        bool jumpExit = false;
        for (int index = 1; index <= timeSegments.size(); ++index) {
            if (nextIndex && nextIndex != index) {
                continue;
            }
            auto obj = timeSegments[index - 1];
            if (valueTime < obj.first) {
                jumpExit = true;
                break;
            }
            nextIndex = 0;
            newIndex = index;
            items[obj.second] += 1;
        }
        if (jumpExit) {
            break;
        }
        valueTime -= maxDropTm;
    }
    cout << "remain:" << valueTime << "|index:" << newIndex << endl;
    return items;
}

int main() {
    int lastIndex = 2;
    auto items = calcHookDropItem(lastIndex, 45);
    for (const auto& item : items) {
        std::cout << item.first << "|" << item.second << "    ";
    }
    std::cout << std::endl;
    return 0;
}
