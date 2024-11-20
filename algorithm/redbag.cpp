#include <iostream>
#include "../util/random.h"

vector<int64_t> redBagSplite(int64_t tgem, int64_t tpart) {
    auto tmpPart = tpart;
    vector<int64_t> result(tpart, 1);
    auto remGem = tgem - tpart * 1;
    if (remGem) {
        for (int64_t i = 0; i < tpart - 1; ++i) {
            int64_t maxGem = remGem / tmpPart * 2;
            int64_t amount = random32(maxGem);
            result[i] += amount;

            remGem -= amount;
            tmpPart--;
        }
        if (remGem > 0) {
            result[tpart - 1] += remGem;
        }
    }
    return result;
}

void test() {
    int64_t total = 0;
    auto values = redBagSplite(100, 20);
    for (auto &val : values) {
        total += val;
        cout << val << "   ";
    }
    cout << endl;
    cout << values.size() << "|" <<  total << endl;
}

int main() {
    int val = 10;
    while (val--) {
        test();
    }
    return 0;
}